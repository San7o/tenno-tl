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
    /**
     * @brief Whether the expected object has a value
     */
    bool has_val;
    /**
     * @brief The value of the expected object
     */
    const T val;
    /**
     * @brief The unexpected value of the expected object
     */
    const unexpected<E> unex;

    /**
     * @brief Construct a new expected object
     *
     * @param val The value of the expected object
     */
    constexpr expected(const T val_in) : has_val(true), val(val_in), unex(E())
    {
    }
    /**
     * @brief Construct a new expected object
     *
     * @param unex The unexpected value of the expected object
     */
    constexpr expected(const unexpected<E> unex_val)
        : has_val(false), val(T()), unex(unex_val)
    {
    }

    /**
     * @brief Converts the expected object to a boolean
     *
     * @return true If the expected object has a value
     * @return false If the expected object does not have a value
     */
    constexpr explicit operator bool() const noexcept
    {
        return has_val;
    }

    /**
     * @brief Compares two expected objects
     *
     * @param other The other expected object to compare to
     * @return true If the two expected objects are equal
     * @return false If the two expected objects are not equal
     */
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

    /**
     * @brief Compares two expected objects
     *
     * @param other The other expected object to compare to
     * @return false If the two expected objects are equal
     * @return true If the two expected objects are not equal
     */
    constexpr bool operator!=(const expected<T, E> &other) const noexcept
    {
        return !(*this == other);
    }

    /**
     * @brief Assigns an expected object to another expected object
     *
     * @param other The other expected object to assign to
     * @return expected<T, E>& The assigned expected object
     */
    constexpr auto
    operator=(const expected<T, E> &other) noexcept -> expected<T, E> &
    {
        has_val = other.has_val;
        val = other.val;
        unex = other.unex;
        return *this;
    }

    /**
     * @brief Checks if the expected object has a value
     *
     * @return true If the expected object has a value
     * @return false If the expected object does not have a value
     */
    constexpr bool has_value() const noexcept
    {
        return has_val;
    }

    /**
     * @brief Returns the value of the expected object
     *
     * @return T The value of the expected object
     */
    constexpr T value() const noexcept
    {
        return val;
    }

    /**
     * @brief Returns the unexpected value of the expected object
     *
     * @return E The unexpected value of the expected object
     */
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
