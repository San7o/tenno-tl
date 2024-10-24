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

#include <tenno/functional.hpp>
#include <tenno/utility.hpp> // tenno::move

namespace tenno
{

/**
 * @brief An unexpected value
 *
 * @tparam E The type of the unexpected value
 */
template <typename E> class unexpected
{
  public:
    const E unex;
    constexpr unexpected() : unex(E())
    {
    }
    constexpr unexpected(const E unex_in) : unex(unex_in)
    {
    }
};

/**
 * @brief An expected value
 *
 * @tparam T The type of the expected value
 * @tparam E The type of the unexpected value
 */
template <typename T, typename E> class expected
{
  public:
    bool has_val;
    const T val;
    const unexpected<E> unex;

    constexpr expected(const T val_in) : has_val(true), val(val_in), unex(E())
    {
    }

    constexpr expected(const unexpected<E> unex_val)
        : has_val(false), val(T()), unex(unex_val)
    {
    }

    constexpr explicit operator bool() const noexcept
    {
        return has_val;
    }

    constexpr bool operator==(const expected<T, E> &other) const noexcept
    {
        if (has_val != other.has_val)
        {
            return false;
        }
        if (has_val)
        {
            return val == other.val;
        }
        return unex.unex == other.unex.unex;
    }

    constexpr bool operator!=(const expected<T, E> &other) const noexcept
    {
        return !(*this == other);
    }

    constexpr auto
    operator=(const expected<T, E> &other) noexcept -> expected<T, E> &
    {
        has_val = other.has_val;
        val = other.val;
        unex = other.unex;
        return *this;
    }

    constexpr bool has_value() const noexcept
    {
        return has_val;
    }

    constexpr T value() const noexcept
    {
        return val;
    }

    constexpr E error() const noexcept
    {
        return unex.unex;
    }

    /**
     * @brief Returns the value of the expected object or a default value
     *
     * @param default_val The default value to return if the expected object
     * does not have a value
     * @return T The value of the expected object or the default value
     */
    constexpr T value_or(const T default_val) const noexcept
    {
        if (has_val)
        {
            return val;
        }
        return default_val;
    }

    /**
     * @brief Returns the unexpected value of the expected object or a default
     * value
     *
     * @param default_err The default value to return if the expected object
     * does not have an unexpected value
     * @return E The unexpected value of the expected object or the default
     * value
     */
    constexpr E error_or(const E default_err) const noexcept
    {
        if (!has_val)
        {
            return unex.unex;
        }
        return default_err;
    }

    // TODO:
    // - and_then
    // - transform
    // - or_else
    // - transform_error
};

} // namespace tenno
