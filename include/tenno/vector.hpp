// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <initializer_list>
#include <tenno/algorithm.hpp>
#include <tenno/error.hpp>
#include <tenno/expected.hpp>
#include <tenno/functional.hpp>
#include <tenno/memory.hpp>
#include <tenno/ranges.hpp>
#include <tenno/types.hpp>
#include <utility> // std::forward

namespace tenno
{

template <class T, class Allocator = tenno::allocator<T>> class vector
{
public:
  using value_type = T;
  using allocator_type = Allocator;
  using size_type = tenno::size;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = typename Allocator::pointer;
  using const_pointer = typename Allocator::const_pointer;
  using difference_type = T;

  constexpr vector()
      : _size(0), _capacity(0), _data(nullptr),
        _allocator(tenno::allocator<T>()) {};

  explicit constexpr vector(size_type count,
                  const T &value, const Allocator &alloc = Allocator())
    : _size(0),
      _capacity(count),
      _allocator(alloc)
  {
    _data = _allocator.allocate(_capacity);
    for (; _size < count; ++_size)
    {
      new (&_data[_size]) T(value);
    }
  }
  explicit constexpr vector(size_type count, const Allocator &alloc = Allocator())
    : _size(0),
      _capacity(count),
      _allocator(alloc)
  {
    _data = _allocator.allocate(_capacity);
    for (; _size < count; ++_size)
    {
      new (&_data[_size]) T();
    }
  }

  // Copy constructor
  constexpr vector(const vector &other, const Allocator &alloc = Allocator())
    : _size(0),
      _capacity(other._capacity),
      _allocator(alloc)

  {
    _data = _allocator.allocate(_capacity);
    for (; _size < other._size; ++_size)
    {
      new (&_data[_size]) T(other._data[_size]);
    }
  }

  // Move constructor
  constexpr vector(vector &&other)
      : _size(other._size), _capacity(other._capacity),
        _data(other._data), _allocator(tenno::move(other._allocator))
  {
    other._data      = nullptr;
    other._size      = 0;
    other._capacity  = 0;
  }

  // Initializer list constructor
  constexpr vector(std::initializer_list<T> init,
                   const Allocator &alloc = Allocator())
      : _size(0), _capacity(init.size()),
        _allocator(alloc)
  {
    _data = _allocator.allocate(_capacity);
    for (const auto& item : init) {
      new (&_data[_size]) T(item);
      ++_size;
    }
  }

  ~vector()
  {
    if (_data == nullptr)
      return;

    for (size_type i = 0; i < _size; ++i)
    {
      _data[i].~T();
    }
    _allocator.deallocate(_data, _capacity);
  }

  constexpr vector &operator=(const vector &other)
  {
    if (this != &other)
    {
      vector temp(other);
      this->swap(temp);
    }
    
    return *this;
  }

  constexpr vector &operator=(vector &&other) noexcept
  {
    if (this != &other)
    {
      this->clear();
      _allocator.deallocate(_data, _capacity);
      
      _size      = other._size;
      _capacity  = other._capacity;
      _data      = other._data;
      _allocator = std::move(other._allocator);

      other._size     = 0;
      other._capacity = 0;
      other._data     = nullptr;
    }
    return *this;
  }

  constexpr vector &operator=(std::initializer_list<value_type> ilist)
  {
    this->clear();

    if (ilist.size() > _capacity)
    {
      _allocator.deallocate(_data, _capacity);
      _data = _allocator.allocate(ilist.size());  
      _capacity = ilist.size();
    }

    _size = 0;
    for (const auto& item : ilist)
    {
      new (&_data[_size]) T(item);
      ++_size;
    }
    return *this;
  }

  void assign(size_type count, const T &value)
  {
    if (count > _capacity)
    {
      T* new_data = _allocator.allocate(count);
      
      this->clear();
      _allocator.deallocate(_data, _capacity);
      
      _data     = new_data;
      _capacity = count;
    }
    else
    {
      this->clear();
    }

    _size = 0;
    for (size_type i = 0; i < count; ++i)
    {
      new (&_data[_size]) T(value);
      ++_size;
    }
  }

  void assign(std::initializer_list<T> ilist)
  {
    if (ilist.size() > _capacity)
    {
      T* new_data = _allocator.allocate(ilist.size());
      
      this->clear();
      _allocator.deallocate(_data, _capacity);
      
      _data     = new_data;
      _capacity = ilist.size();
    }
    else
    {
      this->clear();
    }

    _size = 0;
    for (const auto& i : ilist)
    {
      new (&_data[_size]) T(i);
      ++_size;
    }
  }

  void assign_range(const tenno::range<T> &r)
  {
    if (r.size() > _capacity)
    {
      T* new_data = _allocator.allocate(r.size());
      
      this->clear();
      _allocator.deallocate(_data, _capacity);
      
      _data     = new_data;
      _capacity = r.size();
    }
    else
    {
      this->clear();
    }

    _size = 0;
    for (const auto& i : r)
    {
      new (&_data[_size]) T(i);
      ++_size;
    }
  }

  allocator_type get_allocator() const
  {
    return this->_allocator;
  }

  expected<tenno::reference_wrapper<T>, tenno::error> at(size_type pos)
  {
    if (pos >= _size)
    {
      return tenno::unexpected(tenno::error::out_of_range);
    }
    return tenno::reference_wrapper<T>(_data[pos]);
  }

  // Unsafe!
  T& operator[](size_type pos)
  {
    return _data[pos];
  }

  // Unsafe!
  const T& operator[](size_type pos) const
  {
    return _data[pos];
  }

  expected<tenno::reference_wrapper<const T>, tenno::error> front() const
  {
    if (_size == 0)
    {
      return tenno::unexpected(tenno::error::empty);
    }
    if (_data == nullptr)
    {
      return tenno::unexpected(tenno::error::not_initialized);
    }
    return tenno::reference_wrapper<const T>(_data[0]);
  }

  expected<tenno::reference_wrapper<const T>, tenno::error> back() const
  {
    if (_size == 0)
    {
      return tenno::unexpected(tenno::error::empty);
    }
    if (_data == nullptr)
    {
      return tenno::unexpected(tenno::error::not_initialized);
    }
    return tenno::reference_wrapper<const T>(_data[_size - 1]);
  }

  tenno::expected<pointer, tenno::error> data() noexcept
  {
    if (_data == nullptr)
    {
      return tenno::unexpected(tenno::error::not_initialized);
    }
    return _data;
  }

  tenno::expected<const_pointer, tenno::error> data() const noexcept
  {
    if (_data == nullptr)
    {
      return tenno::unexpected(tenno::error::not_initialized);
    }
    return _data;
  }

  /**
   * @brief An iterator to iterate over the data
   */
  struct iterator
  {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;

    tenno::size index;
    tenno::vector<T, Allocator> &vec;

    explicit iterator(tenno::vector<T, Allocator> &_vec,
                      const tenno::size _index)
        : index(_index), vec(_vec)
    {
    }

    iterator &operator++() noexcept
    {
      index++;
      return *this;
    }

    iterator operator++(int) noexcept
    {
      iterator _iterator = *this;
      ++index;
      return _iterator;
    }

    bool operator==(const iterator &other) const noexcept
    {
      return index == other.index;
    }

    bool operator!=(const iterator &other) const noexcept
    {
      return !(index == other.index);
    }

    T& operator*() noexcept
    {
      return vec[index];
    }

    T* operator->() noexcept
    {
      return &vec[index];
    }

    iterator operator-(iterator &other) const noexcept
    {
      return this->index - other;
    }

    iterator operator+(const size_type &other) const noexcept
    {
      return this->index + other;
    }

    operator tenno::size() const noexcept 
    {
      return this->index;
    }

    T& get() noexcept
    {
      return vec[index];
    }
  };

  /**
   * @brief Get an iterator to the beginning of the data
   *
   * @return iterator The iterator to the beginning of the data
   *
   * # Example
   * ```cpp
   * tenno::vector<int> vec = {1, 2, 3};
   * auto begin = vec.begin();
   * ```
   */
  iterator begin() noexcept
  {
    return iterator(*this, tenno::size(0));
  }

  /**
   * @brief Get an iterator to the end of the data
   *
   * @return iterator The iterator to the end of the data
   *
   * # Example
   * ```cpp
   * tenno::for_each(vec.begin(), vec.end(), [](int& elem) { elem = 0; });
   * ```
   */
  iterator end() noexcept
  {
    return iterator(*this, this->_size);
  }

  /**
   * @brief An iterator to iterate over the data
   */
  struct const_iterator
  {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = const T;
    using pointer = const T *;
    using reference = const T &;

    tenno::size index;
    const tenno::vector<T, Allocator> &vec;

    const_iterator(const iterator& other) 
      : index(other.index), vec(other.vec) {}
    
    explicit const_iterator(const tenno::vector<T, Allocator> &_vec,
                            const tenno::size _index)
        : index(_index), vec(_vec)
    {
    }

    const_iterator &operator++() noexcept
    {
      index++;
      return *this;
    }

    const_iterator operator++(int) noexcept
    {
      const_iterator _iterator = *this;
      ++index;
      return _iterator;
    }

    bool operator==(const const_iterator &other) const noexcept
    {
      return index == other.index;
    }

    bool operator!=(const const_iterator &other) const noexcept
    {
      return !(index == other.index);
    }

    const T& operator*() const noexcept
    {
      return vec[index];
    }

    T* operator->() const noexcept
    {
      return &vec[index];
    }

    size_type operator-(const_iterator &other) const noexcept
    {
      return other.index - this->index;
    }
    
    const T& get() const noexcept
    {
      return vec[index];
    }
  };

  /**
   * @brief Get a const_iterator to the beginning of the data
   *
   * @return const_iterator The const_iterator to the beginning of the data
   *
   * # Example
   * ```cpp
   * tenno::vector<int> vec = {1, 2, 3};
   * auto begin = vec.begin();
   * ```
   */
  const_iterator begin() const noexcept
  {
    return const_iterator(*this, tenno::size(0));
  }

  /**
   * @brief Get a const_iterator to the end of the data
   *
   * @return const_iterator The const_iterator to the end of the data
   *
   * # Example
   * ```cpp
   * tenno::for_each(vec.begin(), vec.end(), [](int& elem) { elem = 0; });
   * ```
   */
  const_iterator end() const noexcept
  {
    return const_iterator(*this, this->_size);
  }

  /**
   * @brief An reverse_iterator to iterate over the data
   */
  struct reverse_iterator
  {
    using reverse_iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;

    long long int index;
    tenno::vector<T, Allocator> &vec;

    explicit reverse_iterator(tenno::vector<T, Allocator> &_vec,
                              const long long int _index)
        : index(_index), vec(_vec)
    {
    }

    reverse_iterator &operator++() noexcept
    {
      index--;
      return *this;
    }

    reverse_iterator operator++(int) noexcept
    {
      reverse_iterator _iterator = *this;
      --index;
      return _iterator;
    }

    bool operator==(const reverse_iterator &other) const noexcept
    {
      return index == other.index;
    }

    bool operator!=(const reverse_iterator &other) const noexcept
    {
      return !(index == other.index);
    }

    T& operator*() noexcept
    {
      return vec[index];
    }

    T* operator->() noexcept
    {
      return &vec[index];
    }

    T& get() noexcept
    {
      return vec[index];
    }
  };

  /**
   * @brief Get an reverse_iterator to the beginning of the data
   *
   * @return reverse_iterator The iterator to the beginning of the data
   *
   * # Example
   * ```cpp
   * tenno::vector<int> vec = {1, 2, 3};
   * auto begin = vec.rbegin();
   * ```
   */
  reverse_iterator rbegin() noexcept
  {
    return reverse_iterator(*this, _size - 1);
  }

  /**
   * @brief Get an reverse_iterator to the end of the data
   *
   * @return reverse_iterator The iterator to the end of the data
   *
   * # Example
   * ```cpp
   * tenno::for_each(vec.rbegin(), vec.rend(), [](int& elem) { elem = 0; });
   * ```
   */
  reverse_iterator rend() noexcept
  {
    return reverse_iterator(*this, -1);
  }

  bool empty() const noexcept
  {
    return _size == 0;
  }

  size_type size() const noexcept
  {
    return _size;
  }

  size_type max_size() const noexcept
  {
    return (tenno::size) -1;
  }

  void reserve(size_type new_cap)
  {
    if (new_cap <= _capacity)
    {
      return;
    }
    
    pointer new_data = _allocator.allocate(new_cap);

    // Move elements into the new memory
    for (size_type i = 0; i < _size; ++i)
    {
      new (&new_data[i]) T(std::move(_data[i]));
    }
    
    // Explicitly destroy the old objects
    for (size_type i = 0; i < _size; ++i)
    {
      _data[i].~T();
    }
    
    _allocator.deallocate(_data, _capacity);
    _data     = new_data;
    _capacity = new_cap;
  }

  size_type capacity() const noexcept
  {
    return _capacity;
  }

  void shrink_to_fit() noexcept
  {
    if (_size == _capacity)
    {
      return;
    }
    pointer new_data = _allocator.allocate(_size);
    
    // Move elements into the new memory
    for (size_type i = 0; i < _size; ++i)
    {
      new (&new_data[i]) T(std::move(_data[i]));
    }
    
    // Explicitly destroy the old objects
    for (size_type i = 0; i < _size; ++i)
    {
      _data[i].~T();
    }
    
    _allocator.deallocate(_data, _capacity);
    
    _data = new_data;
    _capacity = _size;
  }

  void clear() noexcept
  {
    for (size_type i = 0; i < _size; ++i)
    {
      _data[i].~T();
    }
    this->_size = 0;
  }

  constexpr void push_back(const T &value) noexcept
  {
    if (_size == _capacity)
    {
      reserve(_capacity == 0 ? 1 : _capacity * 2);
    }
    
    new (&_data[_size]) T(value);
    _size++;
  }

  constexpr void push_back(T &&value) noexcept
  {
    if (_size == _capacity)
    {
      reserve(_capacity == 0 ? 1 : _capacity * 2);
    }
    new (&_data[_size]) T(std::move(value)); 
    _size++;
  }

  template <class... Args> reference emplace_back(Args &&...args) noexcept
  {
    if (_size >= _capacity)
    {
      size_type new_cap = (_capacity == 0) ? 1 : _capacity * 2;
      this->reserve(new_cap);
    }

    new (&_data[_size]) T(std::forward<Args>(args)...);
    return _data[_size++];
  }

  void pop_back() noexcept
  {
    if (_size == 0)
    {
      return;
    }

    _data[_size - 1].~T();
    _size--;
  }

  void resize(size_type count) noexcept
  {
    if (count < _size)
    {
      for (size_type i = count; i < _size; ++i)
      {
        _data[i].~T();
      }
    }
    else if (count > _size)
    {
      reserve(count);
      for (size_type i = _size; i < count; ++i)
      {
        new (&_data[i]) T();
      }
    }
    _size = count;
  }

  void resize(size_type count, const value_type &value) noexcept
  {
    if (count < _size)
    {
      for (size_type i = count; i < _size; ++i)
      {
        _data[i].~T();
      }
    }
    else if (count > _size)
    {
      reserve(count);
      for (size_type i = _size; i < count; ++i)
      {
        new (&_data[i]) T(value);
      }
    }
    _size = count;
  }

  void swap(vector &other) noexcept
  {
    tenno::swap(_size, other._size);
    tenno::swap(_capacity, other._capacity);
    tenno::swap(_data, other._data);
    tenno::swap(_allocator, other._allocator);
  }

  template<class InputIt>
  iterator insert(const_iterator pos, InputIt first, InputIt last)
  {
    size_type insert_idx = pos.index;
    size_type count = 0;
    for (InputIt it = first; it != last; ++it)
    {
      count++;
    }

    if (count == 0) return iterator(*this, insert_idx);

    if (_size + count > _capacity)
    {
      reserve(tenno::max(_size + count, _capacity * 2));
    }

    size_type old_size = _size;
    _size += count;

    for (size_type i = old_size; i > insert_idx; --i)
    {
      size_type src_idx = i - 1;
      size_type dst_idx = i + count - 1;

      if (dst_idx >= old_size)
      {
        new (&_data[dst_idx]) T(tenno::move(_data[src_idx]));
      }
      else
      {
        _data[dst_idx] = tenno::move(_data[src_idx]);
      }
    }

    InputIt current_input = first;
    for (size_type i = 0; i < count; ++i)
    {
      size_type dst_idx = insert_idx + i;
        
      if (dst_idx >= old_size)
      {
        new (&_data[dst_idx]) T(*current_input);
      }
      else
      {
        _data[dst_idx] = *current_input;
      }
      ++current_input;
    }

    return iterator(*this, insert_idx);
  }
  
private:
  
  size_type _size;
  size_type _capacity;
  pointer   _data;
  Allocator _allocator;
  
};

} // namespace tenno
