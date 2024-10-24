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

#include <cstddef>  /* for std::ptrdiff_t */
#include <iterator> /* for std::forward_iterator_tag */
#include <tenno/types.hpp>

namespace tenno
{

/**
 * @brief A range of elements
 *
 * @tparam T The type of the elements in the range
 */
template <typename T> class range
{
  public:
    T start_elem;
    T end_elem;

    constexpr range(T start, T end) : start_elem(start), end_elem(end)
    {
    }

    constexpr explicit range(T end) : start_elem(0), end_elem(end)
    {
    }

    constexpr tenno::size size() const noexcept
    {
        return (tenno::size) end_elem - start_elem;
    }

    struct iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        T current;
        constexpr explicit iterator(T current_it) : current(current_it)
        {
        }

        constexpr T operator*() const noexcept
        {
            return current;
        }

        constexpr iterator &operator++() noexcept
        {
            ++current;
            return *this;
        }

        constexpr iterator operator++(int) noexcept
        {
            iterator _iterator = *this;
            ++*this;
            return _iterator;
        }

        constexpr bool operator!=(const iterator &other) const noexcept
        {
            return current != other.current;
        }
    };

    constexpr iterator begin() const noexcept
    {
        return iterator(start_elem);
    }

    constexpr iterator end() const noexcept
    {
        return iterator(end_elem);
    }

}; // class range

} // namespace tenno
