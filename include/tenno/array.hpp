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

#include <cstddef>          /* for std::ptrdiff_t */
#include <initializer_list> /* brace-enclosed initializer list must use std */
#include <iterator>         /* for std::forward_iterator_tag */
#include <tenno/algorithm.hpp>
#include <tenno/error.hpp>
#include <tenno/expected.hpp>
#include <tenno/ranges.hpp>
#include <tenno/types.hpp>

namespace tenno
{

/**
 * @brief A fixed-size array
 *
 * @tparam T The type of the elements in the array
 * @tparam N The size of the array
 *
 * # Example
 * ```cpp
 * auto arr = tenno::array<int, 5>();
 * ```
 * No memory allocation is performed, and the array is stored on the stack.
 */
template <typename T, tenno::size N> class array
{
  public:
    /**
     * @brief The type of the elements in the array
     */
    using value_type = T;

    /**
     * @brief Construct a new array object
     *
     * By default, the array's elements are not initialized for
     * performance reasons. If you want to initialize the array's
     * elements, use the static `init` method.
     */
    constexpr array() = default;

    /**
     * @brief Construct a new array object with a list of elements
     *
     * @param list The list of elements to initialize the array with
     *
     * # Example
     * ```cpp
     * tenno::array<int, 5> arr{1, 2, 3, 4, 5};
     * tenno::array<int, 5> arr2 = {5, 4, 3, 2, 1};
     * ```
     */
    constexpr array(std::initializer_list<T> list)
    {
        tenno::copy(list.begin(), list.end(), this->_data);
    }

    /**
     * @brief Get the size of the array
     *
     * @return tenno::size The size of the array
     *
     * # Example
     * ```cpp
     * auto arr = tenno::array<int, 5>();
     * const tenno::size size = arr.size();
     * ```
     */
    constexpr tenno::size size() const noexcept
    {
        return this->_size;
    }

    /**
     * @brief Initialize all elements of the array to the default value of T
     *
     * # Example
     * ```cpp
     * auto arr = tenno::array<int, 10>::init();
     * ```
     */
    constexpr static tenno::array<T, N> init() noexcept
    {
        auto arr = tenno::array<T, N>();
        tenno::for_each(arr.begin(), arr.end(), [](T &elem) { elem = T(); });
        return arr;
    }

    /**
     * @brief Access an element of the array
     *
     * @param index The index of the element to access
     * @return tenno::expected<T, tenno::error> The element at the index
     *
     * # Example
     * ```cpp
     * auto arr = tenno::array<int, 5>{1, 2, 3, 4, 5};
     * auto elem = arr.at(2);
     * if (elem.has_value())
     * {
     *     std::cout << elem.value() << std::endl;
     * }
     * ```
     */
    constexpr tenno::expected<T, tenno::error>
    at(const tenno::size index) const noexcept
    {
        if (index >= this->_size)
        {
            return tenno::unexpected(tenno::error::out_of_range);
        }
        return this->_data[index];
    }

    /**
     * @brief Access an element of the array
     *
     * @param index The index of the element to access
     * @return T The element at the index
     *
     * # Example
     * ```cpp
     * auto arr = tenno::array<int, 5>{1, 2, 3, 4, 5};
     * auto elem = arr[2];
     * ```
     */
    constexpr T front() const noexcept
    {
        return this->_data[0];
    }

    /**
     * @brief Access an element of the array
     *
     * @param index The index of the element to access
     * @return T The element at the index
     *
     * # Example
     * ```cpp
     * auto arr = tenno::array<int, 5>{1, 2, 3, 4, 5};
     * auto elem = arr[2];
     * ```
     */
    constexpr T back() const noexcept
    {
        return this->_data[this->_size - 1];
    }

    /**
     * @brief Access an element of the array
     *
     * @param index The index of the element to access
     * @return T The element at the index
     *
     * # Example
     * ```cpp
     * auto arr = tenno::array<int, 5>{1, 2, 3, 4, 5};
     * auto elem = arr[2];
     * ```
     */
    T *data() noexcept
    {
        return this->_data;
    }

    /**
     * @brief Access the underlying data of the array
     * @return const T* The underlying data of the array
     */
    constexpr const T *data() const noexcept
    {
        return this->_data;
    }

    /**
     * @brief Check if the array is empty
     *
     * @return true If the array is empty
     * @return false If the array is not empty
     *
     * # Example
     * ```cpp
     * auto arr = tenno::array<int, 5>();
     * if (arr.empty())
     * {
     *     std::cout << "The array is empty" << std::endl;
     * }
     * ```
     */
    constexpr bool empty() const noexcept
    {
        return this->_size == 0;
    }

    /**
     * @brief Get the maximum size of the array
     *
     * @return tenno::size The maximum size of the array
     *
     * # Example
     * ```cpp
     * auto arr = tenno::array<int, 5>();
     * const tenno::size max_size = arr.max_size();
     * ```
     */
    constexpr tenno::size max_size() const noexcept
    {
        return this->_size;
    }

    /**
     * @brief Fill the array with a value
     *
     * @param value The value to fill the array with
     *
     * # Example
     * ```cpp
     * auto arr = tenno::array<int, 5>();
     * arr.fill(0);
     * ```
     */
    void fill(const T &value) noexcept
    {
        tenno::for_each(this->begin(), this->end(),
                        [&value](T &elem) { elem = value; });
    }

    /**
     * @brief Swap the contents of the array with another array
     *
     * @param other The other array to swap with
     *
     * # Example
     * ```cpp
     * auto arr = tenno::array<int, 3>{1, 2, 3};
     * auto arr2 = tenno::array<int, 3>{4, 5, 6};
     * arr.swap(arr2);
     * ```
     */
    void swap(array &other) noexcept
    {
        for (auto i : tenno::range<tenno::size>(this->_size))
        {
            auto temp = this->_data[i];
            this->_data[i] = other._data[i];
            other._data[i] = temp;
        }
    }

    /**
     * @brief Access an element of the array
     *
     * @param index The index of the element to access
     * @return T The element at the index
     *
     * # Example
     * ```cpp
     * auto arr = tenno::array<int, 5>{1, 2, 3, 4, 5};
     * auto elem = arr[2];
     * ```
     */
    T &operator[](const tenno::size index) noexcept
    {
        return this->_data[index];
    }

    /**
     * @brief Access an element of the array constexpr
     */
    constexpr T operator[](const tenno::size index) const noexcept
    {
        return this->_data[index];
    }

    /**
     * @brief An iterator to iterate over the array
     */
    struct iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        T *ptr;

        /**
         * @brief Construct a new iterator object
         *
         * @param ptr The pointer to the element the iterator points to
         */
        constexpr iterator(T *ptr_in) : ptr(ptr_in)
        {
        }

        /**
         * @brief Construct a new iterator object
         *
         * @param ptr The pointer to the element the iterator points to
         */
        constexpr iterator(const T *ptr_in) : ptr(const_cast<T *>(ptr_in))
        {
        }

        /**
         * @brief Construct a new iterator object
         *
         * @param other The other iterator to copy
         */
        constexpr iterator &operator++() noexcept
        {
            ptr++;
            return *this;
        }

        /**
         * @brief Construct a new iterator object
         *
         * @param other The other iterator to copy
         */
        constexpr iterator operator++(T) noexcept
        {
            iterator _iterator = *this;
            ++*this;
            return _iterator;
        }

        /**
         * @brief Construct a new iterator object
         *
         * @param other The other iterator to copy
         */
        constexpr bool operator==(const iterator &other) const noexcept
        {
            return ptr == other.ptr;
        }

        /**
         * @brief Construct a new iterator object
         *
         * @param other The other iterator to copy
         */
        constexpr bool operator!=(const iterator &other) const noexcept
        {
            return !(*this == other);
        }

        /**
         * @brief dereference operator
         *
         * @return reference The reference to the element the iterator points to
         */
        constexpr reference operator*() const noexcept
        {
            return *ptr;
        }
    };

    /**
     * @brief Get an iterator to the beginning of the array
     *
     * @return iterator The iterator to the beginning of the array
     *
     * # Example
     * ```cpp
     * auto arr = tenno::array<int, 5>();
     * auto begin = arr.begin();
     * ```
     */
    constexpr iterator begin() const noexcept
    {
        return iterator(this->_data);
    }
    /**
     * @brief Get an iterator to the end of the array
     *
     * @return iterator The iterator to the end of the array
     *
     * # Example
     * ```cpp
     * tenno::for_each(arr.begin(), arr.end(), [](int& elem) { elem = 0; });
     * ```
     */
    constexpr iterator end() const noexcept
    {
        return iterator(this->_data + this->_size);
    }

    struct reverse_iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        T *ptr;

        /**
         * @brief Construct a new reverse_iterator object
         *
         * @param ptr The pointer to the element the iterator points to
         */
        constexpr reverse_iterator(T *ptr_in) : ptr(ptr_in)
        {
        }

        /**
         * @brief Construct a new reverse_iterator object
         *
         * @param ptr The pointer to the element the iterator points to
         */
        constexpr reverse_iterator(const T *ptr_in)
            : ptr(const_cast<T *>(ptr_in))
        {
        }

        /**
         * @brief Construct a new reverse_iterator object
         *
         * @param other The other reverse_iterator to copy
         */
        constexpr reverse_iterator &operator++() noexcept
        {
            ptr--;
            return *this;
        }

        /**
         * @brief Construct a new reverse_iterator object
         *
         * @param other The other reverse_reverse_iterator to copy
         */
        constexpr reverse_iterator operator++(T) noexcept
        {
            reverse_iterator _reverse_reverse_iterator = *this;
            ++*this;
            return _reverse_reverse_iterator;
        }

        /**
         * @brief Construct a new reverse_iterator object
         *
         * @param other The other reverse_reverse_iterator to copy
         */
        constexpr bool operator==(const reverse_iterator &other) const noexcept
        {
            return ptr == other.ptr;
        }

        /**
         * @brief Construct a new reverse_iterator object
         *
         * @param other The other reverse_iterator to copy
         */
        constexpr bool operator!=(const reverse_iterator &other) const noexcept
        {
            return !(*this == other);
        }

        /**
         * @brief dereference operator
         *
         * @return reference The reference to the element the reverse_Iterator
         * points to
         */
        constexpr reference operator*() const noexcept
        {
            return *ptr;
        }
    };

    /**
     * @brief Get a reverse_iterator to the beginning of the array
     *
     * @return reverse_iterator The reverse_iterator to the beginning of the
     * array
     *
     * # Example
     * ```cpp
     * auto arr = tenno::array<int, 5>();
     * auto rbegin = arr.rbegin();
     * ```
     */
    constexpr reverse_iterator rbegin() const noexcept
    {
        return reverse_iterator(this->_data + this->_size - 1);
    }

    /**
     * @brief Get a reverse_iterator to the end of the array
     *
     * @return reverse_iterator The reverse_iterator to the end of the array
     *
     * # Example
     * ```cpp
     * tenno::for_each(arr.rbegin(), arr.rend(), [](int& elem) { elem = 0; });
     * ```
     */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
    constexpr reverse_iterator rend() const noexcept
    {
        return reverse_iterator(this->_data - 1);
    }
#pragma GCC diagnostic pop

  private:
    T _data[N];
    const tenno::size _size = N;

}; // class array

} // namespace tenno
