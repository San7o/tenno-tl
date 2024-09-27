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

#include <utility>

namespace tenno
{

/**
 * @brief Copies the elements in the range [first, last) to the range beginning
 * at d_first.
 *
 * @tparam InputIt The type of the iterator to the first element in the range to
 * copy.
 * @tparam OutputIt The type of the iterator to the first element in the range
 * to copy to.
 * @param first The iterator to the first element in the range to copy.
 * @param last The iterator to the element after the last element in the range
 * to copy.
 * @param d_first The iterator to the first element in the range to copy to.
 * @return OutputIt The iterator to the element after the last element copied.
 */
template <typename InputIt, typename OutputIt>
constexpr OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
{
    while (first != last)
    {
        *d_first++ = *first++;
    }
    return d_first;
}

/**
 * @brief Applies the given function f to the result of dereferencing every
 * element in the range [first, last).
 *
 * @tparam InputIt The type of the iterator to the first element in the range to
 * apply the function to.
 * @tparam UnaryFunc The type of the function to apply to the elements in the
 * range.
 * @param first The iterator to the first element in the range to apply the
 * function to.
 * @param last The iterator to the element after the last element in the range
 * to apply the function to.
 * @param f The function to apply to the elements in the range.
 * @return UnaryFunc The function f.
 */
template <class InputIt, class UnaryFunc>
constexpr UnaryFunc for_each(InputIt first, InputIt last, UnaryFunc f)
{
    while (first != last)
    {
        f(*first++);
    }
    return f;
}

/**
 * @brief Applies the given function f to the result of dereferencing every
 * element in the range [first, last) and returns the result.
 *
 * @tparam InputIt The type of the iterator to the first element in the range to
 * apply the function to.
 * @tparam UnaryFunc The type of the function to apply to the elements in the
 * range.
 * @tparam T The type of the result of the function.
 * @param first The iterator to the first element in the range to apply the
 * function to.
 * @param last The iterator to the element after the last element in the range
 * to apply the function to.
 * @param f The function to apply to the elements in the range.
 * @param init The initial value of the result.
 * @return T The result of the function.
 */
template <class InputIt, class T>
constexpr T accumulate(InputIt first, InputIt last, T init) noexcept
{
    while (first != last)
    {
        init += *first++;
    }
    return init;
}

template <class T> void swap(T &a, T &b) noexcept
{
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}

} // namespace tenno
