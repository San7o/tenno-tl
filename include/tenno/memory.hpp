// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <tenno/mutex.hpp>
#include <tenno/types.hpp>
#include <tenno/utility.hpp>
#include <type_traits>

namespace tenno
{

/*
// Example of a custom allocator
template <class T>
struct custom_allocator
{
  using value_type = T;
  custom_allocator() noexcept;
  template <class U> custom_allocator (const custom_allocator<U>&) noexcept;
  T* allocate (tenno::size n);
  void deallocate (T* p, tenno::size n);
  constexpr bool operator== (const custom_allocator&) const noexcept;
  constexpr bool operator!= (const custom_allocator&) const noexcept;
};
*/

template <class T> struct allocator
{
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_referemce = const T &;
  using size_type = tenno::size;

  allocator() noexcept = default;
  
  template <class U> allocator(const allocator<U> &) noexcept
  {
  }

  T *allocate(tenno::size n)
  {
    return (T *) ::operator new(n * sizeof(T));
  }

  void deallocate(T *p, tenno::size n)
  {
    ::operator delete(p, n * sizeof(T));
  }

  constexpr bool operator==(const allocator &) const noexcept
  {
    return true;
  }

  constexpr bool operator!=(const allocator &) const noexcept
  {
    return false;
  }
};

template <typename T> struct default_delete
{
  default_delete() noexcept = default;
  
  void operator()(T *ptr) const noexcept
  {
    delete ptr;
  }
  
  template <typename U> void operator()(U *ptr) const noexcept
  {
    delete ptr;
  }
};

template <class T> class weak_ptr;

struct control_block_base
{
  long num_ptrs         = 0;
  long num_weak_ptrs    = 0;
  tenno::mutex cb_mutex = {};

  // Deallocate this control block
  // Only call this when num_ptrs == 0 AND num_weak_ptrs == 0
  virtual void deallocate()     = 0;
  // Free the stored object
  virtual void dispose_object() = 0;

  virtual void set_ptr(void* ptr) = 0;
  virtual void* get_ptr() = 0;
  
  virtual ~control_block_base() = default;
};

/**
 * @brief A shared pointer
 *
 * @tparam T The type of the object to point to
 */
template <class T, class Deleter = tenno::default_delete<T>,
          class Alloc = tenno::allocator<T>>
class shared_ptr
{
public:

  /**
   * @brief The type of the object to point to
   */
  using element_type = T;

  struct control_block : control_block_base
  {
    T            *object;
    Alloc         allocator;
    Deleter       deleter;

    control_block() = default;

    control_block(T* obj) : object(obj)
    {
      this->num_ptrs     = 1;
      this->num_num_ptrs = 0;
    }
    
    control_block(T&& obj)
    {
      void* raw_mem   = tenno::allocator<T>().allocate(1);
      T* new_object   = new (raw_mem) T(tenno::move(obj));
      
      this->object = new_object;
      this->num_ptrs = 1;
      this->num_num_ptrs = 0;
    }
    
    void dispose_object() override
    {
      T* delete_me = nullptr;
      {
        tenno::lock_guard<tenno::mutex> lock(cb_mutex);
        if (!this->object) return;
        delete_me = this->object;
        this->object = nullptr;
      }
      this->deleter(delete_me);
    }

    void deallocate() override
    {
      this->~control_block();
      operator delete(this); 
    }

    void set_ptr(void* ptr) override
    {
      this->object = static_cast<T*>(ptr);
    }

    void* get_ptr() override
    {
      return static_cast<void*>(this->object);
    }
    
  };

  friend control_block;
  friend class tenno::weak_ptr<T>;
  template <class Y, class... Args>
  friend typename std::enable_if<!std::is_array<Y>::value, shared_ptr<Y>>::type
  make_shared(Args &&...args) noexcept;
  template <class Y> friend shared_ptr<Y> make_shared() noexcept;
  template <class Y> friend shared_ptr<Y> make_shared(Y&&) noexcept;
  template <class Y>
  friend typename std::enable_if<std::is_array<Y>::value, shared_ptr<Y>>::type
  make_shared(tenno::size n) noexcept;
  // Make sure that all versions of shared_ptr are friends with each
  // other
  template <typename U, typename D, typename A> friend class shared_ptr;
      
  
  /**
   * @brief Default constructor
   */
  shared_ptr() noexcept : _object(nullptr), _control_block(nullptr)
  {
  }

  /**
   * @brief Construct a new shared pointer object
   *
   * @param ptr The object to point to
   */
  shared_ptr(T *ptr)
  {
    size_t         size_in_ts = (sizeof(control_block) + sizeof(T) - 1) / sizeof(T);
    void*          raw_mem    = tenno::allocator<T>().allocate(size_in_ts);
    control_block* cb         = new (raw_mem) control_block();
    
    cb->object           = ptr;
    cb->num_ptrs         = 1;
    cb->num_weak_ptrs    = 0;
    cb->allocator        = tenno::allocator<T>();
    cb->deleter          = tenno::default_delete<T>();
    cb->cb_mutex         = tenno::mutex();
    this->_object        = cb->object;
    this->_control_block = cb;
  }

  /**
   * @brief Construct a shared pointer object by move
   *
   * @param obj Move reference to the object
   */
  shared_ptr(T&& obj)
  {
    size_t         size_in_ts = (sizeof(control_block) + sizeof(T) - 1) / sizeof(T);
    void*          raw_mem    = tenno::allocator<T>().allocate(size_in_ts);
    control_block* cb         = new (raw_mem) control_block(tenno::move(obj));

    cb->num_ptrs         = 1;
    cb->num_weak_ptrs    = 0;
    cb->allocator        = tenno::allocator<T>();
    cb->deleter          = tenno::default_delete<T>();
    cb->cb_mutex         = tenno::mutex();
    this->_object        = cb->object;
    this->_control_block = cb;
  }

  
  /**
   * @brief Construct a new shared pointer object
   *
   * @param ptr The object to point to
   * @param deleter The deleter to use to delete the object
   */
  shared_ptr(T *ptr, Deleter deleter)
  {
    size_t         size_in_ts = (sizeof(control_block) + sizeof(T) - 1) / sizeof(T);
    void*          raw_mem    = tenno::allocator<T>().allocate(size_in_ts);
    control_block* cb         = new (raw_mem) control_block();
    
    cb->object           = ptr;
    cb->num_ptrs         = 1;
    cb->num_weak_ptrs    = 0;
    cb->allocator        = tenno::allocator<T>();
    cb->deleter          = deleter;
    cb->cb_mutex         = tenno::mutex();
    this->_object       = cb->object;
    this->_control_block = cb;
  }

  /**
   * @brief Construct a new shared pointer object
   *
   * @param ptr The object to point to
   * @param deleter The deleter to use to delete the object
   * @param allocator The allocator to use to allocate the control block
   */
  shared_ptr(T *ptr, Deleter deleter, Alloc alloc)
  {
    size_t         size_in_ts = (sizeof(control_block) + sizeof(T) - 1) / sizeof(T);
    void*          raw_mem    = alloc.allocate(size_in_ts);
    control_block* cb         = new (raw_mem) control_block();
    
    cb->object           = ptr;
    cb->num_ptrs         = 1;
    cb->num_weak_ptrs    = 0;
    cb->allocator        = alloc;
    cb->deleter          = deleter;
    cb->cb_mutex         = tenno::mutex();
    this->_object       = cb->object;
    this->_control_block = cb;
  }

  /**
   * @brief Copy constructor
   *
   * @param other The shared pointer to copy
   */
  shared_ptr(const shared_ptr &other) noexcept
      : _object(other._object),
        _control_block(other._control_block),
        do_cache(other.do_cache)
  {
    if (!this->_control_block) return;
    
    tenno::lock_guard<tenno::mutex> lock(this->_control_block->cb_mutex);
    this->_control_block->num_ptrs++;
  }

  /**
   * @brief Move constructor
   *
   * @param other The shared pointer to move
   */
  shared_ptr(shared_ptr &&other) noexcept
      : _object(other._object),
        _control_block(other._control_block),
        do_cache(other.do_cache)
  {
    other._object       = nullptr;
    other._control_block = nullptr;
  }

  /**
   * @brief Conversion constructor
   * This allows shared_ptr<Derived> to be passed where shared_ptr<Base> is expected.
   */
  template <typename U>
  shared_ptr(const shared_ptr<U>& other) noexcept 
    : _object(other._object),
      _control_block(other._control_block),
      do_cache(other.do_cache)
  {
    if (this->_control_block)
      this->_control_block->num_ptrs++;
  }

  /**
   * @brief Aliasing constructor
   */
  template <typename U>
  shared_ptr(const shared_ptr<U>& other, T* object) noexcept 
    : _object(object),
      _control_block(other._control_block),
      do_cache(other.do_cache)
  {
    if (this->_control_block)
      this->_control_block->num_ptrs++;
  }
  
  /**
   * @brief Conversion move constructor
   */
  template <typename U>
  shared_ptr(shared_ptr<U>&& other) noexcept 
    : _object(other._object),
      _control_block(other._control_block),
      do_cache(other.do_cache)
  {
    other._object       = nullptr;
    other._control_block = nullptr;
  }

  /**
   * @brief Destructor
   */
  ~shared_ptr()
  {
    this->reset();
  }

  /**
   * @brief Move assignment operator
   *
   * @param other The shared pointer to move
   * @return shared_ptr& The moved shared pointer
   */
  shared_ptr &operator=(shared_ptr &&other) noexcept
  {
    if (this == &other)
      return *this;

    this->reset();
    
    this->_object        = other._object;
    this->_control_block = other._control_block;
    this->do_cache       = other.do_cache;
    
    other._object        = nullptr;
    other._control_block = nullptr;
    return *this;
  }

  /**
   * @brief Copy assignment operator
   *
   * @param other The shared pointer to copy
   * @return shared_ptr& The copied shared pointer
   */
  shared_ptr &operator=(const shared_ptr &other) noexcept
  {
    this->reset();
    
    this->_object        = other._object;
    this->_control_block = other._control_block;
    this->do_cache       = other.do_cache;
    
    if (this->_control_block)
    {
      tenno::lock_guard<tenno::mutex> lock(this->_control_block->cb_mutex);
      this->_control_block->num_ptrs++;
    }
    return *this;
  }

  /**
   * @brief Templated Move Assignment
   * Allows shared_ptr<Base> = shared_ptr<Derived>
   */
  template<class Y>
  shared_ptr& operator=(shared_ptr<Y>&& other) noexcept
  {
    this->reset(); 

    this->_object        = other._object; // This works if U* converts to T*
    this->_control_block = other._control_block;
    this->do_cache       = other.do_cache;
    
    other._object        = nullptr;
    other._control_block = nullptr;
    
    return *this;
  }

  /**
   * @brief Templated Copy Assignment
   */
  template<class Y>
  shared_ptr& operator=(const shared_ptr<Y>& other) noexcept
  {
    this->reset();

    this->_object        = other._object;
    this->_control_block = other._control_block;
    this->do_cache       = other.do_cache;
    
    if (this->_control_block)
    {
      tenno::lock_guard<tenno::mutex> lock(this->_control_block->cb_mutex);
      this->_control_block->num_ptrs++;
    }
    return *this;
  }

  /**
   * @brief Reset the shared pointer
   */
  void reset() noexcept
  {
    if (!this->_control_block)
      return;

    bool destroy_object        = false;
    bool destroy_control_block = false;

    {
      tenno::lock_guard<tenno::mutex> lock(this->_control_block->cb_mutex);
      this->_control_block->num_ptrs--;

      if (this->_control_block->num_ptrs == 0)
      {
        destroy_object = true;
        
        if (this->_control_block->num_weak_ptrs == 0)
        {
          destroy_control_block = true;
        }
      }
    }

    if (destroy_object)
      this->_control_block->dispose_object();

    if (destroy_control_block)
      this->_control_block->deallocate();

    this->_object       = nullptr;
    this->_control_block = nullptr;
  }
  
  /**
   * @brief Swap the shared pointer with another shared pointer
   *
   * @param other The shared pointer to swap with
   */
  void swap(shared_ptr &other) noexcept
  {
    T                  *tmp_object        = this->_object;
    control_block_base *tmp_control_block = this->_control_block;
    bool                tmp_do_cache      = this->do_cache;

    this->_object        = other._object;
    this->_control_block = other._control_block;
    this->do_cache       = other.do_cache;
    
    other._object        = tmp_object;
    other._control_block = tmp_control_block;
    other.do_cache       = tmp_do_cache;
  }

  /**
   * @brief Swap only the object inside the shared pointers
   *
   * This function does not swap the reference counting of the object,
   * but it swaps the internal pointer to the object.
   */
  void swap_ptr(shared_ptr &other) noexcept
  {
    void *tmp_object = this->_control_block->get_ptr();

    this->_object                = other._control_block->get_ptr();
    this->_control_block->set_ptr(other->_control_block->get_ptr());

    other._object                = tmp_object;
    other._control_block->set_ptr(tmp_object);
  }

  /**
   * @brief Enable or disable pointer caching
   *
   * Of pointer caching is enabled, the shared pointer will hold
   * a copy of a pointer to the T object directly. This makes it
   * possible to access the pointer without dereferencing the
   * inner control block, which improves performance.
   *
   * You can disable this if you need the shared pointer to always
   * reference only the pointer stored in the inner control block.
   */
  void set_cache(bool should_do_cache) noexcept
  {
    this->do_cache = should_do_cache;
    return;
  }

  bool get_cache() const noexcept
  {
    return this->do_cache;
  }
  
  /**
   * @brief Get the object pointed to by the shared pointer
   *
   * @return T* The object pointed to by the shared pointer
   */
  T *get() const noexcept
  {
    return (this->do_cache)
      ? this->_object
      : static_cast<T*>(this->_control_block->get_ptr());
  }

  /**
   * @brief Dereference operator
   *
   * @return T& The object pointed to by the shared pointer
   */
  T &operator*() const noexcept
  {
    return (this->do_cache)
      ? *this->_object
      : *static_cast<T*>(this->_control_block->get_ptr());
  }

  /**
   * @brief Member access operator
   *
   * @return T* The object pointed to by the shared pointer
   */
  T *operator->() const noexcept
  {
    return (this->do_cache)
      ? this->_object
      : static_cast<T*>(this->_control_block->get_ptr());
  }

  /**
   * @brief Array access operator
   *
   * @param index The index of the element to access
   * @return T& The element at the given index
   */
  auto &operator[](tenno::size index) const noexcept
  {
    return (this->do_cache)
      ? (*this->_object)[index]
      : (*static_cast<T*>(this->_control_block->get_ptr()))[index];
  }

  /**
   * @brief Get the number of shared pointers pointing to the object
   *
   * @return long The number of shared pointers pointing to the object
   */
  long use_count() const noexcept
  {
    if (!this->_control_block)
      return 0;
    return this->_control_block->num_ptrs;
  }

  /**
   * @brief Check if the shared pointer is empty
   *
   * @return true If the shared pointer is empty
   * @return false If the shared pointer is not empty
   */
  explicit operator bool() const noexcept
  {
    return (this->do_cache)
      ? this->_object != nullptr
      : static_cast<T*>(this->_control_block->get_ptr()) != nullptr;
  }

  /**
   * @brief Stored value ordering
   *
   * @param other The shared pointer to compare with
   * @return true If the shared pointer is before the other shared pointer
   * @return false If the shared pointer is after the other shared pointer
   */
  bool owner_before(const shared_ptr &other) const noexcept
  {
    return this->_control_block < other._control_block;
  }

  /**
   * @brief Stored value equality
   *
   * @param other The shared pointer to compare with
   * @return true If the shared pointer is equal to the other shared pointer
   * @return false If the shared pointer is not equal to the other shared
   * pointer
   */
  bool owner_equal(const shared_ptr &other) const noexcept
  {
    return this->_control_block == other._control_block;
  }

  // TODO:
  // - owner_hash (c++26)

#ifndef TENNO_DEBUG
private:
#endif
  
  T                  *_object;
  control_block_base *_control_block;

  // Wether to use _object or _control_block->object when accessing the
  // object
  bool do_cache = true;
  
};

/**
 * @brief A weak pointer
 *
 * @tparam T The type of the object to point to
 */
template <typename T> class weak_ptr
{
public:
  /**
   * @brief The type of the object to point to
   */
  using element_type = T;

  template <typename Y> friend class weak_ptr;
  
  /**
   * @brief Default constructor
   */
  constexpr weak_ptr() noexcept
  {
    this->_control_block  = nullptr;
    this->_object         = nullptr;
  }

  /**
   * @brief Copy constructor
   *
   * @param r The weak pointer to copy
   */
  weak_ptr(const weak_ptr &r) noexcept
  {
    if (!r._control_block)
    {
      this->_control_block    = nullptr;
      this->_object           = nullptr;
      this->do_cache          = r.do_cache;
      return;
    }

    this->_control_block    = r._control_block;
    this->_object           = r._object;
    this->do_cache          = r.do_cache;
    
    tenno::lock_guard<tenno::mutex> lock(r._control_block->cb_mutex);
    this->_control_block->num_weak_ptrs++;

  }

  /**
   * @brief Move constructor
   *
   * @param r The weak pointer to move
   */
  weak_ptr(weak_ptr &&r) noexcept
  {
    if (!r._control_block)
    {
      this->_control_block = nullptr;
      this->_object           = nullptr;
      return;
    }

    this->_control_block    = r._control_block;
    this->_object           = r._object;
    this->do_cache          = r.do_cache;
    r._control_block        = nullptr;
    r._object               = nullptr;
  }

  /**
   * @brief Construct a new weak pointer object
   *
   * @param r The shared pointer to copy
   */
  weak_ptr(const tenno::shared_ptr<T> &r) noexcept
  {
    if (!r._control_block)
    {
      this->_control_block = nullptr;
      this->_object           = nullptr;
      return;
    }

    this->_object           = r._object;
    this->_control_block    = r._control_block;
    this->do_cache          = r.do_cache;
    
    tenno::lock_guard<tenno::mutex> lock(r._control_block->cb_mutex);
    this->_control_block->num_weak_ptrs++;
  }

  /**
   * @brief Destructor
   */
  ~weak_ptr()
  {
    if (!this->_control_block) return;

    bool should_delete_block = false;
    {
      tenno::lock_guard<tenno::mutex> lock(this->_control_block->cb_mutex);
      this->_control_block->num_weak_ptrs--;

      if (this->_control_block->num_ptrs == 0 && 
          this->_control_block->num_weak_ptrs == 0)
      {
        should_delete_block = true;
      }
    }

    if (should_delete_block)
    {
      this->_control_block->deallocate();
    }
  }

  /**
   * @brief Copy assignment operator
   *
   * @param r The weak pointer to copy
   * @return weak_ptr& The copied weak pointer
   */
  weak_ptr &operator=(const weak_ptr &r) noexcept
  {
    if (this == &r) return *this;
    
    if (!r._control_block)
    {
      this->_control_block = nullptr;
      this->_object           = nullptr;
      return *this;
    }
    
    this->_control_block    = r._control_block;
    this->_object           = r._object;
    this->do_cache          = r.do_cache;
    
    tenno::lock_guard<tenno::mutex> lock(r._control_block->cb_mutex);
    this->_control_block->num_weak_ptrs++;
    
    return *this;
  }

  /**
   * @brief Move assignment operator
   *
   * @param r The weak pointer to move
   * @return weak_ptr& The moved weak pointer
   */
  weak_ptr &operator=(const shared_ptr<T> &r) noexcept
  {
    if (!r._control_block)
    {
      this->_control_block = nullptr;
      this->_object           = nullptr;
      return *this;
    }

    this->_object           = r._object;
    this->_control_block    = r._control_block;
    this->do_cache          = r.do_cache;
    
    tenno::lock_guard<tenno::mutex> lock(r._control_block->cb_mutex);
    this->_control_block->num_weak_ptrs++;
    
    return *this;
  }

  /**
   * @brief Move assignment operator
   *
   * @param r The weak pointer to move
   * @return weak_ptr& The moved weak pointer
   */
  weak_ptr &operator=(weak_ptr &&r) noexcept
  {
    if (!r._control_block)
    {
      this->_control_block = nullptr;
      this->_object           = nullptr;
      return *this;
    }

    this->_control_block  = r._control_block;
    this->_object         = r._object;
    this->do_cache        = r.do_cache;
    
    r._control_block      = nullptr;
    r._object             = nullptr;
    
    return *this;
  }

  /**
   * @brief Reset the weak pointer
   */
  void reset() noexcept
  {
    bool should_deallocate = false;
    if (this->_control_block)
    {
      tenno::lock_guard<tenno::mutex> lock(this->_control_block->cb_mutex);
      this->_control_block->num_weak_ptrs--;
      if (this->_control_block->num_ptrs == 0
          && this->_control_block->num_weak_ptrs == 0)
        should_deallocate = true;
    }
    if (should_deallocate)
      this->_control_block->deallocate();

    this->_control_block    = nullptr;
    this->_object           = nullptr;
  }

  /**
   * @brief Swap the weak pointer with another weak pointer
   *
   * @param r The weak pointer to swap with
   */
  void swap(weak_ptr &r) noexcept
  {
    auto tmp_cb          = this->_control_block;
    auto tmp_object      = this->_object;
    auto tmp_do_cache    = this->do_cache;
    
    this->_control_block = r._control_block;
    this->_object        = r._object;
    this->do_cache       = r.do_cache;
    
    r._control_block     = tmp_cb;
    r._object            = tmp_object;
    r.do_cache           = tmp_do_cache;
  }

  /**
   * @brief Swap only the object inside the pointers
   *
   * This function does not swap the reference counting of the object,
   * but it swaps the internal pointer to the object.
   */
  void swap_ptr(weak_ptr &other) noexcept
  {
    void *tmp_object = this->_control_block->get_ptr();

    this->_object                = other._control_block->get_ptr();
    this->_control_block->set_ptr(other._control_block->get_ptr());

    other._object                = tmp_object;
    other._control_block->set_ptr(tmp_object);
  }

  /**
   * @brief Swap only the object inside the pointers
   *
   * This function does not swap the reference counting of the object,
   * but it swaps the internal pointer to the object.
   */
  void swap_ptr(shared_ptr<T> &other) noexcept
  {
    void *tmp_object = this->_control_block->get_ptr();
    this->_control_block->set_ptr(other._control_block->get_ptr());
    
    other._control_block->set_ptr(tmp_object);
    other._object = static_cast<T*>(tmp_object);
  }
  
  /**
   * @brief Enable or disable pointer caching
   *
   * Of pointer caching is enabled, the shared pointer will hold
   * a copy of a pointer to the T object directly. This makes it
   * possible to access the pointer without dereferencing the
   * inner control block, which improves performance.
   *
   * You can disable this if you need the shared pointer to always
   * reference only the pointer stored in the inner control block.
   */
  void set_cache(bool should_do_cache) noexcept
  {
    this->do_cache = should_do_cache;
    return;
  }

  bool get_cache() const noexcept
  {
    return this->do_cache;
  }
  
  /**
   * @brief Get the number of shared pointers pointing to the object
   *
   * @return long The number of shared pointers pointing to the object
   */
  long use_count() const noexcept
  {
    if (this->_control_block)
      return this->_control_block->num_ptrs;
    return 0;
  }

  /**
   * @brief Check if the weak pointer is empty
   *
   * @return true If the weak pointer is empty
   * @return false If the weak pointer is not empty
   */
  bool expired() const noexcept
  {
    if (!this->_control_block)
      return true;
    return this->_control_block->num_ptrs == 0;
  }

  /**
   * @brief Lock the weak pointer
   *
   * @return shared_ptr<T> The shared pointer to the object
   */
  tenno::shared_ptr<T> lock() const noexcept
  {
    if (!this->_control_block || this->_control_block->num_ptrs == 0)
      return nullptr;

    auto sp           = tenno::shared_ptr<T>();
    sp._object        = this->_object;
    sp._control_block = this->_control_block;
    sp.do_cache       = this->do_cache;
    sp._control_block->num_ptrs++;
    return sp;
  }

  /**
   * @brief Stored value ordering
   *
   * @param other The weak pointer to compare with
   * @return true If the weak pointer is before the other weak pointer
   * @return false If the weak pointer is after the other weak pointer
   */
  template <class Y> bool owner_before(const weak_ptr<Y> &other) const noexcept
  {
    return this->_control_block < other._control_block;
  }

  /**
   * @brief Stored value ordering
   *
   * @param other The shared pointer to compare with
   * @return true If the weak pointer is before the other shared pointer
   * @return false If the weak pointer is after the other shared pointer
   */
  template <class Y>
  bool owner_before(const tenno::shared_ptr<Y> &other) const noexcept
  {
    return this->_control_block < other._control_block;
  }

  /**
   * @brief Stored value equality
   *
   * @param other The weak pointer to compare with
   * @return true If the weak pointer is equal to the other weak pointer
   * @return false If the weak pointer is not equal to the other weak pointer
   */
  template <class Y>
  bool owner_equal(const tenno::shared_ptr<Y> &other) const noexcept
  {
    return this->_control_block == other._control_block;
  }

  // TODO
  /*
  tenno::size_t owner_hash() const noexcept
  {
      return std::hash<element_type>()(this->_ptr);
  }
  */

private:

  element_type         *_object;
  control_block_base   *_control_block;

  // Wether to use _ptr or _control_block->object when accessing the
  // object
  bool do_cache = true;
  
};

/**
 * @brief A shared pointer
 *
 * @tparam T The type of the object to point to
 */
template <class T, class Deleter = tenno::default_delete<T>> class unique_ptr
{
public:
  using pointer      = T *;
  using element_type = T;
  using deleter_type = Deleter;

  /**
   * @brief Construct a new empty unique_ptr object
   */
  constexpr unique_ptr() : _value(nullptr)
  {
  }

  /**
   * @brief Construct a new unique_ptr object with the given pointer
   *
   * @param ptr The pointer to the object
   */
  constexpr unique_ptr(T *ptr, Deleter deleter = tenno::default_delete<T>())
      : _value(ptr), _deleter(deleter)
  {
  }

  constexpr unique_ptr(const unique_ptr &other) = delete;
  /**
   * @brief Construct a new unique_ptr object by moving the other object
   *
   * @param other The other unique_ptr object to move
   */
  constexpr unique_ptr(unique_ptr &&other) noexcept
  {
    this->reset(other.release());
    this->_deleter = other.get_deleter();
  }

  /**
   * @brief Destroy the unique_ptr object
   */
#if __cplusplus >= 202002L // C++20
  constexpr
#endif
    ~unique_ptr()
  {
    if (this->_value != nullptr)
      this->_deleter(this->_value);
  }

  constexpr unique_ptr &operator=(const unique_ptr &other) = delete;
  /**
   * @brief Move the other unique_ptr object into this one
   *
   * @param other The other unique_ptr object to move
   * @return unique_ptr& The reference to this object
   */
  constexpr unique_ptr &operator=(unique_ptr &&other) noexcept
  {
    this->reset(other.release());
    this->_deleter = other.get_deleter();
    return *this;
  }

  /**
   * @brief Release the pointer from the unique_ptr object
   *
   * @return pointer The pointer to the object
   */
  constexpr pointer release() noexcept
  {
    if (!this->_value)
      return nullptr;

    pointer ptr = _value;
    this->_value = nullptr;
    return ptr;
  }

  /**
   * @brief Reset the pointer to the given value
   *
   * @param ptr The pointer to the object
   */
  constexpr void reset(pointer ptr = pointer()) noexcept
  {
    if (this->_value)
      this->_deleter(this->_value);
    this->_value = ptr;
  }

  /**
   * @brief Swap the unique_ptr object with the other one
   *
   * @param other The other unique_ptr object to swap with
   */
  void swap(unique_ptr &other) noexcept
  {
    T tmp_val = *other.get();
    Deleter del_tmp = other.get_deleter();
    *other.get() = *this->get();
    other.get_deleter() = this->get_deleter();
    *this->_value = tmp_val;
    this->_deleter = del_tmp;
  }

  /**
   * @brief Get the pointer to the object
   *
   * @return pointer The pointer to the object
   */
  constexpr pointer get() const noexcept
  {
    return this->_value;
  }

  /**
   * @brief Get the deleter of the object
   *
   * @return deleter_type The deleter of the object
   */
  deleter_type &get_deleter()
  {
    return this->_deleter;
  }

  /**
   * @brief Boolean conversion operator
   * @return true if the pointer is not null, false otherwise
   */
  explicit operator bool() const noexcept
  {
    return this->_value != nullptr;
  }

  /**
   * @brief Dereference operator
   * @return T& The reference to the object
   */
  T &operator*() noexcept
  {
    return *this->_value;
  }

  /**
   * @brief Dereference operator
   * @return T& The reference to the object
   */
  T &operator->() noexcept
  {
    return this->_value;
  }

private:
  
  pointer _value = nullptr;
  deleter_type _deleter;
  
};

/**
 * @brief Create a shared pointer with the given arguments
 *
 * @tparam T The type of the object to create
 * @tparam Args The type of the arguments to pass to the constructor
 * @param args The arguments to pass to the constructor
 * @return shared_ptr<T> The shared pointer to the object
 *
 * @note Make shared takes care of the allocation of the object, while the
 * shared pointer takes care of the allocation and deallocation of the
 * internal control block, as well as the deallocation of the object.
 * @note The object must accept variadic arguments in the constructor
 */
template <class T, class... Args>
typename std::enable_if<!std::is_array<T>::value, shared_ptr<T>>::type
make_shared(Args &&...args) noexcept
{
  using cb_t = typename tenno::shared_ptr<T>::control_block;
  tenno::shared_ptr<T> sp;
  T *t = new T(args...);

  size_t         size_in_ts = (sizeof(cb_t) + sizeof(T) - 1) / sizeof(T);
  void*          raw_mem    = tenno::allocator<T>().allocate(size_in_ts);
  cb_t* cb                  = new (raw_mem) cb_t();
    
  cb->object        = t;
  cb->num_ptrs      = 1;
  cb->num_weak_ptrs = 0;
  cb->allocator     = tenno::allocator<T>();
  cb->deleter       = tenno::default_delete<T>();
  cb->cb_mutex      = tenno::mutex();
  sp._object        = t;
  sp._control_block = cb;
  return tenno::move(sp);
}

template <class T>
typename std::enable_if<std::is_array<T>::value, shared_ptr<T>>::type
make_shared(tenno::size n) noexcept
{
  using cb_t = typename tenno::shared_ptr<T>::control_block;
  tenno::shared_ptr<T> sp;
  T *t = new T[n];

  size_t         size_in_ts = (sizeof(cb_t) + sizeof(T) - 1) / sizeof(T);
  void*          raw_mem    = tenno::allocator<T>().allocate(size_in_ts);
  cb_t* cb                  = new (raw_mem) cb_t();

  cb->object        = t;
  cb->num_ptrs      = 1;
  cb->num_weak_ptrs = 0;
  cb->allocator     = tenno::allocator<T>();
  cb->deleter       = tenno::default_delete<T>();
  cb->cb_mutex      = tenno::mutex();
  sp._object        = t;
  sp._control_block = cb;
  return tenno::move(sp);
}

/**
 * @brief Default initialize a shared pointer
 *
 * @tparam T The type of the object to create
 * @return shared_ptr<T> The shared pointer to the object
 */
template <class T> shared_ptr<T> make_shared() noexcept
{
  using cb_t = typename tenno::shared_ptr<T>::control_block;
  tenno::shared_ptr<T> sp;
  T *t = new T();

  size_t         size_in_ts = (sizeof(cb_t) + sizeof(T) - 1) / sizeof(T);
  void*          raw_mem    = tenno::allocator<T>().allocate(size_in_ts);
  cb_t* cb                  = new (raw_mem) cb_t();

  cb->object        = t;
  cb->num_ptrs      = 1;
  cb->num_weak_ptrs = 0;
  cb->allocator     = tenno::allocator<T>();
  cb->deleter       = tenno::default_delete<T>();
  cb->cb_mutex      = tenno::mutex();
  sp._object        = t;
  sp._control_block = cb;
  return tenno::move(sp);
}

template <class T> shared_ptr<T> make_shared(T&& val) noexcept
{
  using cb_t = typename tenno::shared_ptr<T>::control_block;
  tenno::shared_ptr<T> sp;
  T *t = new T(tenno::move(val));

  size_t         size_in_ts = (sizeof(cb_t) + sizeof(T) - 1) / sizeof(T);
  void*          raw_mem    = tenno::allocator<T>().allocate(size_in_ts);
  cb_t* cb                  = new (raw_mem) cb_t();

  cb->object        = t;
  cb->num_ptrs      = 1;
  cb->num_weak_ptrs = 0;
  cb->allocator     = tenno::allocator<T>();
  cb->deleter       = tenno::default_delete<T>();
  cb->cb_mutex      = tenno::mutex();
  sp._object        = t;
  sp._control_block = cb;
  return tenno::move(sp);
}

/**
 * @brief Create a shared pointer with the given arguments and allocator
 *
 * @tparam T The type of the object to create
 * @tparam Alloc The type of the allocator to use
 * @tparam Args The type of the arguments to pass to the constructor
 * @param alloc The allocator to use
 * @param args The arguments to pass to the constructor
 * @return shared_ptr<T> The shared pointer to the object
 */
template <class T, class Alloc, class... Args>
// typename std::enable_if<!std::is_array<T>::value, shared_ptr<T>>::type
typename tenno::shared_ptr<T> allocate_shared(Alloc &alloc,
                                              Args &&...args) noexcept
{
  void* raw_mem = alloc.allocate(1);
  T *t = new (raw_mem) T(args...);
  return tenno::shared_ptr<T>(t);
}

/**
 * @brief Default initialize a shared pointer with the given allocator
 *
 * @tparam T The type of the object to create
 * @tparam Alloc The type of the allocator to use
 * @param alloc The allocator to use
 * @return shared_ptr<T> The shared pointer to the object
 */
template <class T, class Alloc>
shared_ptr<T> allocate_shared(Alloc &alloc) noexcept
{
  void* raw_mem = alloc.allocate(1);
  T *t = new (raw_mem) T();
  return tenno::shared_ptr<T>(t);
}

/*
template <class T, class Alloc>
typename std::enable_if<std::is_array<T>::value, shared_ptr<T>>::type
allocate_shared(Alloc &alloc, tenno::size n) noexcept
{
    using elem = std::remove_all_extents<T>::type;

    T *t = (T*) alloc.allocate(n);
    auto *cb =
        new tenno::shared_ptr<T, tenno::default_delete<elem>>::control_block();
    cb->object = t;
    cb->num_ptrs = 1;
    cb->num_weak_ptrs = 0;
    cb->allocator = alloc;
    cb->deleter = tenno::default_delete<elem>();
    cb->cb_mutex = tenno::mutex();

    auto sp = tenno::shared_ptr<T, tenno::default_delete<elem>>();
    sp._object = t;
    sp._control_block = cb;
    return tenno::move(sp);
}
*/

/**
 * @brief Create a unique pointer with the given arguments
 *
 * @tparam T The type of the object to create
 * @tparam Args The type of the arguments to pass to the constructor
 * @param args The arguments to pass to the constructor
 * @return unique_ptr<T> The unique pointer to the object
 * @note The object must accept variadic arguments in the constructor
 */
template <class T, class... Args>
constexpr unique_ptr<T> make_unique(Args &&...args)
{
  T *t = new T(args...);
  return tenno::unique_ptr<T>(t);
}

} // namespace tenno
