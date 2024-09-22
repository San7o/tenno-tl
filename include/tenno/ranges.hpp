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

namespace tenno
{

template <typename T> class range
{
  public:
    /**
     * @brief The first element in the range
     */
    T start_elem;
    /**
     * @brief The element after the last element in the range
     */
    T end_elem;

    /**
     * @brief Construct a new range object
     *
     * @param start The first element in the range
     * @param end The element after the last element in the range
     */
    range(T start, T end) : start_elem(start), end_elem(end)
    {
    }

    /**
     * @brief Returns the size of the range
     *
     * @return T The size of the range
     */
    T size() const
    {
        return end_elem - start_elem;
    }

    /**
     * @brief An iterator to iterate over the range
     */
    struct Iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        T current;
        Iterator(T current_it) : current(current_it)
        {
        }

        T operator*() const
        {
            return current;
        }

        Iterator &operator++()
        {
            ++current;
            return *this;
        }

        bool operator!=(const Iterator &other) const
        {
            return current != other.current;
        }
    };

    /**
     * @brief Returns an iterator to the first element in the range
     *
     * @return Iterator The iterator to the first element in the range
     */
    Iterator begin() const
    {
        return Iterator(start_elem);
    }

    /**
     * @brief Returns an iterator to the element after the last element in the
     * range
     *
     * @return Iterator The iterator to the element after the last element in
     * the range
     */
    Iterator end() const
    {
        return Iterator(end_elem);
    }

}; // class range

} // namespace tenno
