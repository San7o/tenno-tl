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

#include <initializer_list> /* brace-enclosed initializer list must use std */
#include <tenno/types.hpp>

namespace tenno
{

template <typename T, tenno::size N>
class array
{
public:

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
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            this->data[it - list.begin()] = *it;
        }
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
    tenno::size size() const noexcept { return this->_size; }

    /**
     * @brief Initialize all elements of the array to the default value of T
     *
     * # Example
     * ```cpp
     * auto arr = tenno::array<int, 10>::init();
     * ```
     */
    static tenno::array<T,N> init() noexcept
    {
        auto arr = tenno::array<T, N>();
        //for (const auto i : tenno::range(0, N))
        for (tenno::size i = 0; i < N; ++i)
        {
            arr.data[i] = T();
        }
        return arr;
    }

private:
    const tenno::size _size = N;
}; // class array

} // namespace tenno
