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
#include <tenno/memory.hpp>
#include <tenno/ranges.hpp>
#include <tenno/types.hpp>
#include <tenno/functional.hpp>
#include <tenno/types.hpp>

namespace tenno
{

template <class T, class Allocator = tenno::allocator<T>> class vector
{
  public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = tenno::size;
    // using difference_type = tenno::difference;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = typename Allocator::pointer;
    using const_pointer = typename Allocator::const_pointer;

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

    constexpr vector &operator=(const vector &other)
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

    constexpr void assign(size_type count, const T &value)
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
    constexpr void assign( InputIt first, InputIt last )
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

    constexpr void assign_range(const tenno::range<T> &r)
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

    expected<tenno::reference_wrapper<T>, tenno::error> operator[](size_type pos)
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

    expected<tenno::reference_wrapper<const T>, tenno::error> operator[](size_type pos) const
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

    /**
     * @brief Get the data object
     * @note Use this at your own risk
     *
     * @return pointer
     */
    constexpr tenno::expected<pointer, tenno::error> data() noexcept
    {
        if (_data == nullptr)
        {
            return tenno::unexpected(tenno::error::not_initialized);
        }
        return _data;
    }

    /**
     * @brief Get the data object
     * @note Use this at your own risk
     *
     * @return const_pointer
     */
    constexpr tenno::expected<const_pointer, tenno::error> data() const noexcept
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
    // TODO: constexpr iterator
    struct iterator_mut
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        tenno::size index;
        tenno::vector<T, Allocator> &vec;

        /**
         * @brief Construct a new iterator object
         *
         * @param ptr The pointer to the element the iterator points to
         */
        explicit iterator(tenno::vector<T, Allocator> &_vec, const tenno::size _index) : index(_index), vec(_vec)
        {
        }

        /**
         * @brief Construct a new iterator object
         *
         * @param other The other iterator to copy
         */
        iterator &operator++() noexcept
        {
            index++;
            return *this;
        }

        /**
         * @brief Construct a new iterator object
         *
         * @param other The other iterator to copy
         */
        iterator operator++(T) noexcept
        {
            iterator _iterator = *this;
            ++index;
            return _iterator;
        }

        /**
         * @brief Construct a new iterator object
         *
         * @param other The other iterator to copy
         */
        bool operator==(const iterator &other) const noexcept
        {
            return index == other.index;
        }

        /**
         * @brief Construct a new iterator object
         *
         * @param other The other iterator to copy
         */
        bool operator!=(const iterator &other) const noexcept
        {
            return !(index == other.index);
        }

        /**
         * @brief dereference operator
         *
         * @return reference The reference to the element the iterator points to
         */
        tenno::expected<tenno::reference_wrapper<T>, tenno::error> operator*() noexcept
        {
            return vec[index];
        }

        tenno::expected<tenno::reference_wrapper<T>, tenno::error> operator->() noexcept
        {
            return vec[index];
        }

        tenno::expected<tenno::reference_wrapper<T>, tenno::error> get() noexcept
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

    // TODO: reverse iterator

    // TODO: Capacity

    constexpr size_type size() const noexcept
    {
        return _size;
    }
    constexpr size_type capacity() const noexcept
    {
        return _capacity;
    }

    // TODO: Modifiers

  private:
    size_type _size;
    size_type _capacity;
    pointer _data;
    Allocator _allocator;
};

} // namespace tenno
