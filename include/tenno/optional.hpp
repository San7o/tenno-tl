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

#include <tenno/algorithm.hpp>
#include <type_traits> // std::is_constant_evaluated

namespace tenno
{

/**
 * @brief An optional value
 *
 * @tparam T The type of the optional value
 * The optional class template manages an optional contained value, i.e. a value
 * that may or may not be present. A common use case for optional is the return
 * value of a function that may fail. It is also usefult for costly-to-construct
 * objects since it avoids the need to construct them until they are needed.
 */
template <typename T> class optional
{
  public:
    /**
     * @brief The type of the optional value
     */
    using value_type = T;

    /**
     * @brief Constructs an optional that does not contain a value
     */
    constexpr optional() noexcept : _has_value(false)
    {
        if (std::is_constant_evaluated())
        {
            _value = T();
        }
    }

    /**
     * @brief Constructs an optional that contains a value
     *
     * @param value The value to contain
     */
    constexpr optional(T value) noexcept : _has_value(true), _value(value)
    {
    }

    /**
     * @brief Constructs an optional that contains a value
     *
     * @param value The value to contain
     */
    optional operator=(T &value) noexcept
    {
        this = optional(value);
    }

    /**
     * @brief Gets the value of the optional
     *
     * @return T The value of the optional
     */
    constexpr T operator->() const noexcept
    {
        return this->_value;
    }

    /**
     * @brief Gets the value of the optional
     *
     * @return T The value of the optional
     */
    constexpr T operator*() const noexcept
    {
        return this->_value;
    }

    /**
     * @brief Checks if the optional has a value
     *
     * @return true If the optional has a value
     * @return false If the optional does not have a value
     */
    constexpr bool has_value() const noexcept
    {
        return this->_has_value;
    }

    /**
     * @brief Returns the value of the optional
     *
     * @return T The value of the optional
     */
    constexpr T value() const noexcept
    {
        return this->_value;
    }

    /**
     * @brief Returns the value of the optional or a default value
     *
     * @param other The default value to return if the optional does not have a
     * value
     * @return T The value of the optional or the default value
     */
    constexpr T value_or(const T &other) const noexcept
    {
        if (this->_has_value)
            return this->_value;
        return other;
    }

    /**
     * @brief Swaps the optional with another optional
     *
     * @param other The other optional to swap with
     */
    void swap(optional &other) noexcept
    {
        if (this->_has_value && other.has_value())
            tenno::swap(*this, other);
    }

    /**
     * @brief Resets the optional
     */
    void reset() noexcept
    {
        if (this->_has_value)
            this->_value.~T();
        *this = optional();
    }

    /**
     * @brief Constructs the value in place
     *
     * @tparam Args The types of the arguments to construct the value
     * @param args The arguments to construct the value
     * @return T& The constructed value
     */
    template <class... Args> T &emplace(Args &&...args)
    {
        *this = optional(T(args...));
        return this->_value;
    }

  private:
    bool _has_value;
    union
    {
        T _value;
    };
};

} // namespace tenno
