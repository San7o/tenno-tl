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

#include <memory> // std::addressof

namespace tenno
{

/**
 * @brief A reference wrapper class
 * @details This class is a wrapper around a reference to an object. It is
 * useful when you need to store a reference in a container, for example.
 *
 * @tparam T
 */
template <class T> class reference_wrapper
{
  public:
    using type = T;
    /**
     * @brief Construct a new reference wrapper object
     *
     * @param x
     */
    constexpr reference_wrapper(T &&x) noexcept : _ptr(std::addressof(x))
    {
    }

    constexpr reference_wrapper() noexcept : _ptr(nullptr)
    {
    }

    constexpr reference_wrapper(const reference_wrapper &other) noexcept
        : _ptr(other._ptr)
    {
    }

    reference_wrapper &operator=(const reference_wrapper &other) noexcept
    {
        _ptr = other._ptr;
        return *this;
    }

    constexpr operator T &() const noexcept
    {
        return *_ptr;
    }

    constexpr T &ref() const noexcept
    {
        return *_ptr;
    }

    constexpr bool operator==(const T &&other) const noexcept
    {
        return *_ptr == other;
    }

    constexpr bool operator!=(const T &&other) const noexcept
    {
        return *_ptr != other;
    }

    // TODO: operator()

  private:
    T *_ptr;
};

} // namespace tenno
