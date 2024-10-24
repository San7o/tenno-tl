/*
 * MIT License
 *
 * Copyright (c) 2024 Giovanni Santini

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

#include <initializer_list>
#include <tenno/error.hpp>
#include <tenno/expected.hpp>
#include <tenno/functional.hpp>
#include <tenno/memory.hpp>
#include <tenno/ranges.hpp>
#include <tenno/types.hpp>
#include <tenno/algorithm.hpp>
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

    vector()
        : _size(0), _capacity(0), _data(nullptr),
          _allocator(tenno::allocator<T>()) {};

    explicit vector(size_type count,

                    const T &value, const Allocator &alloc = Allocator())
        : _size(count), _capacity(count), _data(_allocator.allocate(count)),
          _allocator(alloc)
    {
        for (size_type i = 0; i < count; ++i)
        {
            _data[i] = value;
        }
    }
    explicit vector(size_type count, const Allocator &alloc = Allocator())
        : _size(count), _capacity(count), _data(_allocator.allocate(count)),
          _allocator(alloc)
    {
    }

    vector(const vector &other)
        : _size(other._size), _capacity(other._capacity),
          _data(_allocator.allocate(other._size)), _allocator(other._allocator)
    {
        for (size_type i = 0; i < other._size; ++i)
        {
            _data[i] = other._data[i];
        }
    }

    vector(const vector &other, const Allocator &alloc)
        : _size(other._size), _capacity(other._capacity),
          _data(_allocator.allocate(other._size)), _allocator(alloc)
    {
        for (size_type i = 0; i < other._size; ++i)
        {
            _data[i] = other._data[i];
        }
    }

    vector(vector &&other)
        : _size(other._size), _capacity(other._capacity), _data(other._data),
          _allocator(other._allocator)
    {
        other._size = 0;
        other._data = nullptr;
    }

    vector(vector &&other, const Allocator &alloc)
        : _size(other._size), _capacity(other._capacity), _data(other._data),
          _allocator(alloc)
    {
        other._size = 0;
        other._data = nullptr;
    }
    vector(std::initializer_list<T> init, const Allocator &alloc = Allocator())
        : _size(init.size()), _capacity(init.size()),
          _data(_allocator.allocate(init.size())), _allocator(alloc)
    {
        size_type i = 0;
        for (auto it = init.begin(); it != init.end(); ++it)
        {
            _data[i] = *it;
            ++i;
        }
    }

    ~vector()
    {
        if (_data == nullptr)
            return;
        _allocator.deallocate(_data, _size);
    }

    vector &operator=(const vector &other)
    {
        if (this != &other)
        {
            _allocator.deallocate(_data, _capacity);
            _size = other._size;
            _capacity = other._size;
            _allocator = other._allocator;
            _data = _allocator.allocate(_size);
            for (size_type i = 0; i < _size; ++i)
            {
                _data[i] = other._data[i];
            }
        }
        return *this;
    }

    vector &operator=(vector &&other) noexcept
    {
        if (this != &other)
        {
            _allocator.deallocate(_data, _capacity);
            _size = other._size;
            _capacity = other._capacity;
            _data = other._data;
            _allocator = other._allocator;

            other._size = 0;
            other._data = nullptr;
            other._capacity = 0;
        }
        return *this;
    }

    vector &operator=(std::initializer_list<value_type> ilist)
    {
        _allocator.deallocate(_data, _capacity);
        _size = ilist.size();
        _capacity = ilist.size();
        _data = _allocator.allocate(_size);
        size_type i = 0;
        for (auto it = ilist.begin(); it != ilist.end(); ++it)
        {
            _data[i] = *it;
            ++i;
        }
        return *this;
    }

    void assign(size_type count, const T &value)
    {
        if (count > _capacity)
        {
            _allocator.deallocate(_data, _capacity);
            _size = count;
            _capacity = count;
            _data = _allocator.allocate(_size);
        }
        for (size_type i = 0; i < count; ++i)
        {
            _data[i] = value;
        }
    }

    /*
    // this overwrites the previous assign method
    template< class InputIt >
    void assign( InputIt first, InputIt last )
    {
        size_type count = 0;
        InputIt tmp = first;
        while (tmp != last)
        {
            count++;
            tmp++;
        }

        if (count > _capacity)
        {
            _allocator.deallocate(_data, _capacity);
            _size = count;
            _capacity = count;
            _data = _allocator.allocate(_size);
        }
        size_type i = 0;
        for (auto it = first; it != last; ++it)
        {
            _data[i] = *it;
            ++i;
        }
    }
    */

    void assign(std::initializer_list<T> ilist)
    {
        _allocator.deallocate(_data, _capacity);
        _size = ilist.size();
        _capacity = ilist.size();
        _data = _allocator.allocate(_size);
        size_type i = 0;
        for (auto it = ilist.begin(); it != ilist.end(); ++it)
        {
            _data[i] = *it;
            ++i;
        }
    }

    void assign_range(const tenno::range<T> &r)
    {
        if (r.size() > _capacity)
        {
            _allocator.deallocate(_data, _capacity);
            _size = r.size();
            _capacity = r.size();
            _data = _allocator.allocate(_size);
        }
        size_type i = 0;
        for (auto it = r.begin(); it != r.end(); ++it)
        {
            _data[i] = *it;
            ++i;
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
        return tenno::reference_wrapper<T>(tenno::move(_data[pos]));
    }

    expected<tenno::reference_wrapper<T>, tenno::error>
    operator[](size_type pos)
    {
        if (pos >= _size)
        {
            return tenno::unexpected(tenno::error::out_of_range);
        }
        if (_data == nullptr)
        {
            return tenno::unexpected(tenno::error::not_initialized);
        }
        return tenno::reference_wrapper<T>(tenno::move(_data[pos]));
    }

    expected<tenno::reference_wrapper<const T>, tenno::error>
    operator[](size_type pos) const
    {
        if (pos >= _size)
        {
            return tenno::unexpected(tenno::error::out_of_range);
        }
        if (_data == nullptr)
        {
            return tenno::unexpected(tenno::error::not_initialized);
        }
        return tenno::reference_wrapper<const T>(tenno::move(_data[pos]));
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
        return tenno::reference_wrapper<const T>(tenno::move(_data[0]));
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
        return tenno::reference_wrapper<const T>(tenno::move(_data[_size - 1]));
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

        iterator operator++(T) noexcept
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

        tenno::expected<tenno::reference_wrapper<T>, tenno::error>
        operator*() noexcept
        {
            return vec[index];
        }

        tenno::expected<tenno::reference_wrapper<T>, tenno::error>
        operator->() noexcept
        {
            return vec[index];
        }

        tenno::expected<tenno::reference_wrapper<T>, tenno::error>
        get() noexcept
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

        reverse_iterator operator++(T) noexcept
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

        tenno::expected<tenno::reference_wrapper<T>, tenno::error>
        operator*() noexcept
        {
            return vec[index];
        }

        tenno::expected<tenno::reference_wrapper<T>, tenno::error>
        operator->() noexcept
        {
            return vec[index];
        }

        tenno::expected<tenno::reference_wrapper<T>, tenno::error>
        get() noexcept
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
        for (size_type i = 0; i < _size; ++i)
        {
            new_data[i] = _data[i];
        }
        _allocator.deallocate(_data, _capacity);
        _data = new_data;
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
        for (size_type i = 0; i < _size; ++i)
        {
            new_data[i] = _data[i];
        }
        _allocator.deallocate(_data, _capacity);
        _data = new_data;
        _capacity = _size;
    }

    void clear() noexcept
    {
        this->_size = 0;
    }

    void push_back(const T &value) noexcept
    {
        if (_size == _capacity)
        {
            reserve(_capacity == 0 ? 1 : _capacity * 2);
        }
        _data[_size] = value;
        _size++;
    }

    void push_back(const T &&value) noexcept
    {
        push_back(value);
    }

    template <class... Args> reference emplace_back(Args &&...args) noexcept
    {
        push_back(T(std::forward<Args>(args)...));
        return _data[_size - 1];
    }

    void pop_back() noexcept
    {
        if (_size == 0)
        {
            return;
        }
        _size--;
    }

    void resize(size_type count) noexcept
    {
        reserve(count);
        while (_size < count)
        {
            _data[_size] = T();
            ++_size;
        }
        _size = count;
    }

    void resize(size_type count, const value_type &value) noexcept
    {
        reserve(count);
        while (_size < count)
        {
            _data[_size] = value;
            ++_size;
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

  private:
    size_type _size;
    size_type _capacity;
    pointer _data;
    Allocator _allocator;
};

} // namespace tenno
