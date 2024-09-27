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

namespace tenno
{

template <typename T> class optional
{
  public:
    using value_type = T;

    constexpr optional() noexcept : _has_value(false)
    {
        if (std::is_constant_evaluated())
        {
            _value = T();
        }
    }

    constexpr optional(T value) noexcept : _has_value(true), _value(value)
    {
    }

    optional operator=(T &value) noexcept
    {
        this = optional(value);
    }

    constexpr T operator->() const noexcept
    {
        return this->_value;
    }

    constexpr T operator*() const noexcept
    {
        return this->_value;
    }

    constexpr bool has_value() const noexcept
    {
        return this->_has_value;
    }

    constexpr T value() const noexcept
    {
        return this->_value;
    }

    constexpr T value_or(const T &other) const noexcept
    {
        if (this->_has_value)
            return this->_value;
        return other;
    }

    void swap(optional &other) noexcept
    {
        if (this->_has_value && other.has_value())
            tenno::swap(*this, other);
    }

    void reset() noexcept
    {
        if (this->_has_value)
            this->_value.~T();
        *this = optional();
    }

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
