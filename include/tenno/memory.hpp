/*
 * MIT License
 *
 * Copyright (c) 2024 Giovanni Santini
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

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

    struct control_block
    {
        T *object;
        Alloc allocator;
        Deleter deleter;
        long num_ptrs;
        long num_weak_ptrs;
        tenno::mutex cb_mutex;
        control_block() = default;
        void deallocate()
        {
            this->deleter(this->object);
            if (this->num_weak_ptrs == 0)
                this->allocator.deallocate(
                    (T *) this,
                    sizeof(control_block) / sizeof(T)
                        + (sizeof(control_block) % sizeof(T) != 0 ? 1 : 0));
        }
    };

    friend control_block;
    friend class tenno::weak_ptr<T>;
    template <class Y, class... Args>
    friend typename std::enable_if<!std::is_array<Y>::value, shared_ptr<Y>>::type
    make_shared(Args &&...args) noexcept;
    template <class Y> friend shared_ptr<Y> make_shared() noexcept;
    template <class Y>
    friend typename std::enable_if<std::is_array<Y>::value, shared_ptr<Y>>::type
    make_shared(tenno::size n) noexcept;

    /**
     * @brief Default constructor
     */
    shared_ptr() noexcept : _element(nullptr), _control_block(nullptr)
    {
    }

    /**
     * @brief Construct a new shared pointer object
     *
     * @param ptr The object to point to
     */
    shared_ptr(T *ptr)
    {
        auto *cb = (control_block *) tenno::allocator<T>().allocate(
            sizeof(control_block) / sizeof(T)
            + (sizeof(control_block) % sizeof(T) != 0 ? 1 : 0));
        cb->object = ptr;
        cb->num_ptrs = 1;
        cb->num_weak_ptrs = 0;
        cb->allocator = tenno::allocator<T>();
        cb->deleter = tenno::default_delete<T>();
        cb->cb_mutex = tenno::mutex();
        this->_element = cb->object;
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
        auto *cb = (control_block *) tenno::allocator<T>().allocate(
            sizeof(control_block) / sizeof(T)
            + (sizeof(control_block) % sizeof(T) != 0 ? 1 : 0));
        cb->object = ptr;
        cb->num_ptrs = 1;
        cb->num_weak_ptrs = 0;
        cb->allocator = tenno::allocator<T>();
        cb->deleter = deleter;
        cb->cb_mutex = tenno::mutex();
        this->_element = cb->object;
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
        control_block *cb = (control_block *) alloc.allocate(
            sizeof(control_block) / sizeof(T)
            + (sizeof(control_block) % sizeof(T) != 0 ? 1 : 0));
        cb->object = ptr;
        cb->num_ptrs = 1;
        cb->num_weak_ptrs = 0;
        cb->allocator = alloc;
        cb->deleter = deleter;
        cb->cb_mutex = tenno::mutex();
        this->_element = cb->object;
        this->_control_block = cb;
    }

    /**
     * @brief Copy constructor
     *
     * @param other The shared pointer to copy
     */
    shared_ptr(const shared_ptr &other) noexcept
        : _element(other._element), _control_block(other._control_block)
    {
        tenno::lock_guard<tenno::mutex> lock(this->_control_block->cb_mutex);
        this->_control_block->num_ptrs++;
    }

    /**
     * @brief Move constructor
     *
     * @param other The shared pointer to move
     */
    shared_ptr(shared_ptr &&other) noexcept
        : _element(other._element), _control_block(other._control_block)
    {
        other._element = nullptr;
        other._control_block = nullptr;
    }

    /**
     * @brief Destructor
     */
    ~shared_ptr()
    {
        if (!this->_control_block)
            return;

        {
            tenno::lock_guard<tenno::mutex> lock(
                this->_control_block->cb_mutex);
            this->_control_block->num_ptrs--;
        }
        if (this->_control_block->num_ptrs == 0)
        {
            this->_control_block->deallocate();
        }
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

        this->_element = other._element;
        this->_control_block = other._control_block;
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
        if (this == &other)
            return *this;

        if (this->_control_block)
        {
            {
                tenno::lock_guard<tenno::mutex> lock(
                    this->_control_block->cb_mutex);
                this->_control_block->num_ptrs--;
            }
            if (this->_control_block->num_ptrs == 0)
            {
                this->_control_block->deallocate();
            }
        }

        this->_element = other._element;
        this->_control_block = other._control_block;
        other._control_block->num_ptrs++;
        return *this;
    }

    /**
     * @brief Reset the shared pointer
     */
    void reset() noexcept
    {
        if (!this->_control_block)
            return;

        {
            tenno::lock_guard<tenno::mutex> lock(
                this->_control_block->cb_mutex);
            this->_control_block->num_ptrs--;
        }
        if (this->_control_block->num_ptrs == 0)
        {
            this->_control_block->deallocate();
        }

        this->_element = nullptr;
        this->_control_block = nullptr;
    }

    /**
     * @brief Reset the shared pointer to point to the object managed by the
     * control block cb
     *
     * @param cb The control block to manage the object
     */
    void reset(T *ptr) noexcept
    {
        if (!this->_control_block)
            return;

        {
            tenno::lock_guard<tenno::mutex> lock(
                this->_control_block->cb_mutex);
            this->_control_block->num_ptrs--;
        }
        if (this->_control_block->num_ptrs == 0)
        {
            this->_control_block->deallocate();
        }

        auto *cb = (control_block *) tenno::allocator<T>().allocate(
            sizeof(control_block) / sizeof(T)
            + (sizeof(control_block) % sizeof(T) != 0 ? 1 : 0));
        cb->object = ptr;
        cb->num_ptrs = 1;
        cb->num_weak_ptrs = 0;
        cb->allocator = tenno::allocator<T>();
        cb->deleter = tenno::default_delete<T>();
        cb->cb_mutex = tenno::mutex();
        this->_element = cb->object;
        this->_control_block = cb;
    }

    /**
     * @brief Reset the shared pointer to point to the object managed by the
     * control block cb
     *
     * @param cb The control block to manage the object
     * @param deleter The deleter to use to delete the object
     */
    void reset(T *ptr, Deleter deleter) noexcept
    {
        if (!this->_control_block)
            return;

        {
            tenno::lock_guard<tenno::mutex> lock(
                this->_control_block->cb_mutex);
            this->_control_block->num_ptrs--;
        }
        if (this->_control_block->num_ptrs == 0)
        {
            this->_control_block->deallocate();
        }

        auto *cb = (control_block *) tenno::allocator<T>().allocate(
            sizeof(control_block) / sizeof(T)
            + (sizeof(control_block) % sizeof(T) != 0 ? 1 : 0));
        cb->object = ptr;
        cb->num_ptrs = 1;
        cb->num_weak_ptrs = 0;
        cb->allocator = tenno::allocator<T>();
        cb->deleter = deleter;
        cb->cb_mutex = tenno::mutex();
        this->_element = cb->object;
        this->_control_block = cb;
    }

    /**
     * @brief Reset the shared pointer to point to the object managed by the
     * control block cb
     *
     * @param cb The control block to manage the object
     * @param deleter The deleter to use to delete the object
     * @param allocator The allocator to use to allocate the object
     */
    void reset(T *ptr, Deleter deleter, Alloc alloc) noexcept
    {
        if (!this->_control_block)
            return;

        {
            tenno::lock_guard<tenno::mutex> lock(
                this->_control_block->cb_mutex);
            this->_control_block->num_ptrs--;
        }
        if (this->_control_block->num_ptrs == 0)
        {
            this->_control_block->deallocate();
        }

        control_block *cb = (control_block *) alloc.allocate(
            sizeof(control_block) / sizeof(T)
            + (sizeof(control_block) % sizeof(T) != 0 ? 1 : 0));
        cb->object = ptr;
        cb->num_ptrs = 1;
        cb->num_weak_ptrs = 0;
        cb->allocator = alloc;
        cb->deleter = deleter;
        cb->cb_mutex = tenno::mutex();
        this->_element = cb->object;
        this->_control_block = cb;
    }

    /**
     * @brief Swap the shared pointer with another shared pointer
     *
     * @param other The shared pointer to swap with
     */
    void swap(shared_ptr &other) noexcept
    {
        T *tmp_element = this->_element;
        control_block *tmp_control_block = this->_control_block;

        this->_element = other._element;
        this->_control_block = other._control_block;

        other._element = tmp_element;
        other._control_block = tmp_control_block;
    }

    /**
     * @brief Get the object pointed to by the shared pointer
     *
     * @return T* The object pointed to by the shared pointer
     */
    T *get() const noexcept
    {
        return this->_element;
    }

    /**
     * @brief Dereference operator
     *
     * @return T& The object pointed to by the shared pointer
     */
    T &operator*() const noexcept
    {
        return *this->_element;
    }

    /**
     * @brief Member access operator
     *
     * @return T* The object pointed to by the shared pointer
     */
    T *operator->() const noexcept
    {
        return this->_element;
    }

    /**
     * @brief Array access operator
     *
     * @param index The index of the element to access
     * @return T& The element at the given index
     */
    auto &operator[](tenno::size index) const noexcept
    {
        return (*this->_element)[index];
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
        return this->_element != nullptr;
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
        return *this->_element < *other._element;
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
        if (!this->_element && !other._element)
            return true;
        if (!this->_element || !other._element)
            return false;
        return *this->_element == *other._element;
    }

    // TODO:
    // - owner_hash (c++26)

#ifndef TENNO_DEBUG
  private:
#endif
    T *_element;
    control_block *_control_block;
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

    /**
     * @brief Default constructor
     */
    constexpr weak_ptr() noexcept
    {
        this->_control_block = nullptr;
        this->_ptr = nullptr;
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
            this->_control_block = nullptr;
            this->_ptr = nullptr;
            return;
        }
        this->_control_block = r._control_block;
        this->_ptr = r._ptr;
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
            this->_ptr = nullptr;
            return;
        }
        this->_control_block = r._control_block;
        this->_ptr = r._ptr;
        r._control_block = nullptr;
        r._ptr = nullptr;
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
            this->_ptr = nullptr;
            return;
        }
        tenno::lock_guard<tenno::mutex> lock(r._control_block->cb_mutex);
        this->_ptr = r._element;
        this->_control_block = r._control_block;
        this->_control_block->num_weak_ptrs++;
    }

    /**
     * @brief Destructor
     */
    ~weak_ptr()
    {
        if (this->_control_block)
        {
            {
                tenno::lock_guard<tenno::mutex> lock(
                    this->_control_block->cb_mutex);
                this->_control_block->num_weak_ptrs--;
            }
            if (this->_control_block->num_ptrs == 0
                && this->_control_block->num_weak_ptrs == 0)
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
        if (this->_control_block)
        {
            {
                tenno::lock_guard<tenno::mutex> lock(
                    this->_control_block->cb_mutex);
                this->_control_block->num_weak_ptrs--;
            }
            if (this->_control_block->num_ptrs == 0
                && this->_control_block->num_weak_ptrs == 0)
                this->_control_block->deallocate();
        }
        if (!r._control_block)
        {
            this->_control_block = nullptr;
            this->_ptr = nullptr;
            return *this;
        }
        tenno::lock_guard<tenno::mutex> lock(r._control_block->cb_mutex);
        this->_control_block = r._control_block;
        this->_ptr = r._ptr;
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
        if (this->_control_block)
        {
            {
                tenno::lock_guard<tenno::mutex> lock(
                    this->_control_block->cb_mutex);
                this->_control_block->num_weak_ptrs--;
            }
            if (this->_control_block->num_ptrs == 0
                && this->_control_block->num_weak_ptrs == 0)
                this->_control_block->deallocate();
        }
        if (!r._control_block)
        {
            this->_control_block = nullptr;
            this->_ptr = nullptr;
            return *this;
        }
        tenno::lock_guard<tenno::mutex> lock(r._control_block->cb_mutex);
        this->_ptr = r._element;
        this->_control_block = r._control_block;
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
        if (this->_control_block)
        {
            {
                tenno::lock_guard<tenno::mutex> lock(
                    this->_control_block->cb_mutex);
                this->_control_block->num_weak_ptrs--;
            }
            if (this->_control_block->num_ptrs == 0
                && this->_control_block->num_weak_ptrs == 0)
                this->_control_block->deallocate();
        }
        if (!r._control_block)
        {
            this->_control_block = nullptr;
            this->_ptr = nullptr;
            return *this;
        }
        this->_control_block = r._control_block;
        this->_ptr = r._ptr;
        r._control_block = nullptr;
        r._ptr = nullptr;
        return *this;
    }

    /**
     * @brief Reset the weak pointer
     */
    void reset() noexcept
    {
        if (this->_control_block)
            this->_control_block->num_weak_ptrs--;
        this->_control_block = nullptr;
        this->_ptr = nullptr;
    }

    /**
     * @brief Swap the weak pointer with another weak pointer
     *
     * @param r The weak pointer to swap with
     */
    void swap(weak_ptr &r) noexcept
    {
        auto cb_tmp = this->_control_block;
        auto ptr_tmp = this->_ptr;
        this->_control_block = r._control_block;
        this->_ptr = r._ptr;
        r._control_block = cb_tmp;
        r._ptr = ptr_tmp;
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
        if (this->_control_block->num_ptrs == 0)
            return tenno::shared_ptr<T>();
        auto sp = tenno::shared_ptr<T>();
        sp._element = this->_ptr;
        sp._control_block = this->_control_block;
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
    template <class Y>
    bool owner_before(const weak_ptr<Y> &other) const noexcept
    {
        if (this->_ptr && !other._ptr)
            return true;
        if (!this->_ptr && other._ptr)
            return false;
        if (!this->_ptr || !other._ptr)
            return false;
        return *this->_ptr < *other._ptr;
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
        return *this->_ptr < *other._element;
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
        if (!this->_ptr && !other._element)
            return true;
        if (!this->_ptr || !other._element)
            return false;
        return *this->_ptr == *other._element;
    }

    // TODO
    /*
    tenno::size_t owner_hash() const noexcept
    {
        return std::hash<element_type>()(this->_ptr);
    }
    */

  private:
    element_type *_ptr;
    typename tenno::shared_ptr<T>::control_block *_control_block;
};

/**
 * @brief A shared pointer
 *
 * @tparam T The type of the object to point to
 */
template <class T, class Deleter = tenno::default_delete<T>> class unique_ptr
{
  public:
    using pointer = T *;
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
    cb_t *cb = (cb_t *) tenno::allocator<T>().allocate(
        sizeof(cb_t) / sizeof(T) + (sizeof(cb_t) % sizeof(T) != 0 ? 1 : 0));
    cb->object = t;
    cb->num_ptrs = 1;
    cb->num_weak_ptrs = 0;
    cb->allocator = tenno::allocator<T>();
    cb->deleter = tenno::default_delete<T>();
    cb->cb_mutex = tenno::mutex();
    sp._element = t;
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
    cb_t *cb = (cb_t *) tenno::allocator<T>().allocate(
        sizeof(cb_t) / sizeof(T) + (sizeof(cb_t) % sizeof(T) != 0 ? 1 : 0));
    cb->object = t;
    cb->num_ptrs = 1;
    cb->num_weak_ptrs = 0;
    cb->allocator = tenno::allocator<T>();
    cb->deleter = tenno::default_delete<T>();
    cb->cb_mutex = tenno::mutex();
    sp._element = t;
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
    cb_t *cb = (cb_t *) tenno::allocator<T>().allocate(
        sizeof(cb_t) / sizeof(T) + (sizeof(cb_t) % sizeof(T) != 0 ? 1 : 0));
    cb->object = t;
    cb->num_ptrs = 1;
    cb->num_weak_ptrs = 0;
    cb->allocator = tenno::allocator<T>();
    cb->deleter = tenno::default_delete<T>();
    cb->cb_mutex = tenno::mutex();
    sp._element = t;
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
typename tenno::shared_ptr<T> allocate_shared(Alloc &alloc, Args &&...args) noexcept
{
    T *t = alloc.allocate(1);
    *t = T(args...);
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
    T *t = alloc.allocate(1);
    *t = T();
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
    sp._element = t;
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
