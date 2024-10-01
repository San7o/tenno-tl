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

// TODO: remove this
#include <memory> // for std::allocator and default_delete
#include <tenno/types.hpp>

namespace tenno
{

/**
 * @brief A shared pointer
 *
 * @tparam T The type of the object to point to
 */
template <class T, class Deleter = std::default_delete<T>> class unique_ptr
{
  public:
    using pointer = T *;
    using element_type = T;
    using deleter_type = Deleter;

    /**
     * @brief Construct a new empty unique_ptr object
     */
    constexpr unique_ptr() : _value(nullptr)
    {
    }

    /**
     * @brief Construct a new unique_ptr object with the given pointer
     *
     * @param ptr The pointer to the object
     */
    constexpr unique_ptr(T *ptr, Deleter deleter = std::default_delete<T>())
        : _value(ptr), _deleter(deleter)
    {
    }

    constexpr unique_ptr(const unique_ptr &other) = delete;
    /**
     * @brief Construct a new unique_ptr object by moving the other object
     *
     * @param other The other unique_ptr object to move
     */
    constexpr unique_ptr(unique_ptr &&other) noexcept
    {
        this->reset(other.release());
        this->_deleter = other.get_deleter();
    }

    /**
     * @brief Destroy the unique_ptr object
     */
    constexpr ~unique_ptr()
    {
        if (this->_value != nullptr)
            this->_deleter(this->_value);
    }

    constexpr unique_ptr &operator=(const unique_ptr &other) = delete;
    /**
     * @brief Move the other unique_ptr object into this one
     *
     * @param other The other unique_ptr object to move
     * @return unique_ptr& The reference to this object
     */
    constexpr unique_ptr &operator=(unique_ptr &&other) noexcept
    {
        this->reset(other.release());
        this->_deleter = other.get_deleter();
        return *this;
    }

    /**
     * @brief Release the pointer from the unique_ptr object
     *
     * @return pointer The pointer to the object
     */
    constexpr pointer release() noexcept
    {
        if (!this->_value)
            return nullptr;

        pointer ptr = _value;
        this->_value = nullptr;
        return ptr;
    }

    /**
     * @brief Reset the pointer to the given value
     *
     * @param ptr The pointer to the object
     */
    constexpr void reset(pointer ptr = pointer()) noexcept
    {
        if (this->_value)
            this->_deleter(this->_value);
        this->_value = ptr;
    }

    /**
     * @brief Swap the unique_ptr object with the other one
     *
     * @param other The other unique_ptr object to swap with
     */
    void swap(unique_ptr &other) noexcept
    {
        T tmp_val = *other.get();
        Deleter del_tmp = other.get_deleter();
        *other.get() = *this->get();
        other.get_deleter() = this->get_deleter();
        *this->_value = tmp_val;
        this->_deleter = del_tmp;
    }

    /**
     * @brief Get the pointer to the object
     *
     * @return pointer The pointer to the object
     */
    constexpr pointer get() const noexcept
    {
        return this->_value;
    }

    /**
     * @brief Get the deleter of the object
     *
     * @return deleter_type The deleter of the object
     */
    deleter_type &get_deleter()
    {
        return this->_deleter;
    }

    /**
     * @brief Boolean conversion operator
     * @return true if the pointer is not null, false otherwise
     */
    explicit operator bool() const noexcept
    {
        return this->_value != nullptr;
    }

    /**
     * @brief Dereference operator
     * @return T& The reference to the object
     */
    T &operator*() noexcept
    {
        return *this->_value;
    }

    /**
     * @brief Dereference operator
     * @return T& The reference to the object
     */
    T &operator->() noexcept
    {
        return this->_value;
    }

  private:
    pointer _value = nullptr;
    deleter_type _deleter;
};

} // namespace tenno
