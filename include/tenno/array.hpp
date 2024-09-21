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
     * @brief The underlying array
     */
    T data[N];

    /**
     * @brief Construct a new array object
     *
     * By default, the array's elements are not initialized for
     * performance reasons. If you want to initialize the array's
     * elements, use the static `init` method.
     */
    array() = default;

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
    array(std::initializer_list<T> list)
    {
        tenno::copy(list.begin(), list.end(), this->data);
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
    tenno::size size() const noexcept
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
    static tenno::array<T, N> init() noexcept
    {
        auto arr = tenno::array<T, N>();
        tenno::for_each(arr.begin(), arr.end(), [](T &elem) { elem = T(); });
        return arr;
    }

    

    /**
     * @brief An iterator to iterate over the array
     */
    struct Iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        T *ptr;

        /**
         * @brief Construct a new Iterator object
         *
         * @param ptr The pointer to the element the iterator points to
         */
        Iterator(T *ptr) : ptr(ptr)
        {
        }

        /**
         * @brief Construct a new Iterator object
         *
         * @param other The other iterator to copy
         */
        Iterator &operator++()
        {
            ptr++;
            return *this;
        }

        /**
         * @brief Construct a new Iterator object
         *
         * @param other The other iterator to copy
         */
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }

        /**
         * @brief Construct a new Iterator object
         *
         * @param other The other iterator to copy
         */
        bool operator==(const Iterator &other) const
        {
            return ptr == other.ptr;
        }

        /**
         * @brief Construct a new Iterator object
         *
         * @param other The other iterator to copy
         */
        bool operator!=(const Iterator &other) const
        {
            return !(*this == other);
        }

        /**
         * @brief dereference operator
         *
         * @return reference The reference to the element the iterator points to
         */
        reference operator*() const
        {
            return *ptr;
        }
    };

    /**
     * @brief Get an iterator to the beginning of the array
     *
     * @return Iterator The iterator to the beginning of the array
     *
     * # Example
     * ```cpp
     * auto arr = tenno::array<int, 5>();
     * auto begin = arr.begin();
     * ```
     */
    Iterator begin()
    {
        return Iterator(data);
    }
    /**
     * @brief Get an iterator to the end of the array
     *
     * @return Iterator The iterator to the end of the array
     *
     * # Example
     * ```cpp
     * tenno::for_each(arr.begin(), arr.end(), [](int& elem) { elem = 0; });
     * ```
     */
    Iterator end()
    {
        return Iterator(data + N);
    }

  private:
    const tenno::size _size = N;

}; // class array

} // namespace tenno
