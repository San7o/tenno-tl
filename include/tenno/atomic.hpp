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

#include <tenno/mutex.hpp>

namespace tenno
{

template <typename T> class atomic;

/* general case */
template <typename T> class atomic
{
  public:
    using value_type = T;
    const bool is_always_lock_free = false;

    atomic() noexcept = default;
    ~atomic() noexcept = default;
    atomic(const atomic &) = delete;
    atomic &operator=(const atomic &) = delete;
    atomic &operator=(const atomic &) volatile = delete;

    inline bool is_lock_free() const noexcept
    {
        return this->is_always_lock_free;
    }

    inline void store(T desired) noexcept
    {
        tenno::lock_guard<tenno::mutex> lock(this->_mutex);
        this->_value = desired;
    }

    inline T load() noexcept
    {
        tenno::lock_guard<tenno::mutex> lock(this->_mutex);
        return this->_value;
    }

    operator T() noexcept
    {
        return this->load();
    }

    inline T exchange(T desired) noexcept
    {
        tenno::lock_guard<tenno::mutex> lock(this->_mutex);
        T old = this->_value;
        this->_value = desired;
        return old;
    }

    inline bool compare_exchange_weak(const T &expected, T desired) noexcept
    {
        tenno::lock_guard<tenno::mutex> lock(this->_mutex);
        if (this->_value == expected)
        {
            this->_value = desired;
            return true;
        }
        return false;
    }

    inline bool compare_exchange_strong(const T &expected, T desired) noexcept
    {
        tenno::lock_guard<tenno::mutex> lock(this->_mutex);
        if (this->_value == expected)
        {
            this->_value = desired;
            return true;
        }
        return false;
    }

    // TODO
    // - wait
    // - notify_one
    // - notify_all

  private:
    T _value;
    tenno::mutex _mutex;
};

/* general pointer */
template <typename U> class atomic<U *>
{
  public:
    using value_type = U*;
    const bool is_always_lock_free = false;

    atomic() noexcept = default;
    ~atomic() noexcept = default;
    atomic(const atomic &) = delete;
    atomic &operator=(const atomic &) = delete;
    atomic &operator=(const atomic &) volatile = delete;

    inline bool is_lock_free() const noexcept
    {
        return this->is_always_lock_free;
    }

    inline void store(U* desired) noexcept
    {
        tenno::lock_guard<tenno::mutex> lock(this->_mutex);
        this->_value = desired;
    }

    inline U load() noexcept
    {
        tenno::lock_guard<tenno::mutex> lock(this->_mutex);
        return *this->_value;
    }

    operator U() noexcept
    {
        return this->load();
    }

    inline U exchange(U* desired) noexcept
    {
        tenno::lock_guard<tenno::mutex> lock(this->_mutex);
        U old = *this->_value;
        this->_value = desired;
        return old;
    }

    /**
     * @brief Compare and exchange the value of the atomic object
     * @param expected The expected value
     * @param desired The desired value
     * @return true if the exchange was successful, false otherwise
     */
    inline bool compare_exchange_weak(const U* expected, U* desired) noexcept
    {
        tenno::lock_guard<tenno::mutex> lock(this->_mutex);
        if (this->_value != nullptr && *this->_value == *expected)
        {
            *this->_value = *desired;
            return true;
        }
        return false;
    }

    inline bool compare_exchange_strong(const U* expected, U* desired) noexcept
    {
        tenno::lock_guard<tenno::mutex> lock(this->_mutex);
        if (this->_value != nullptr && *this->_value == *expected)
        {
            *this->_value = *desired;
            return true;
        }
        return false;
    }

    // UODO
    // - wait
    // - notify_one
    // - notify_all

  private:
    U* _value;
    tenno::mutex _mutex;
};

/* int specialization */
/*
 * Read this:
 * https://www.felixcloutier.com/x86/lock
 *
 * Maybe this class extends the general atomic class and overrides the
 * methods that need to be optimized for the int type ??
 */
template <> class atomic<int>
{
  public:
    using value_type = int;
    const bool is_always_lock_free = true;

    atomic() noexcept = default;
    ~atomic() noexcept = default;
    atomic(const atomic &) = delete;
    atomic &operator=(const atomic &) = delete;
    atomic &operator=(const atomic &) volatile = delete;

    inline bool is_lock_free() const noexcept
    {
        return this->is_always_lock_free;
    }

    inline void store(int desired) noexcept
    {
        // make atomic
        this->_value = desired;
    }

    inline int load() const noexcept
    {
        // make atomic
        return this->_value;
    }

    operator int() noexcept
    {
        return this->load();
    }

    inline int exchange(int desired) noexcept
    {
        // read-modify-write operation (CAS)
        return desired;
    }

    inline bool compare_exchange_weak([[maybe_unused]] int &expected,
                                      [[maybe_unused]] int desired) noexcept
    {
        // TODO
        // more efficient and used in weak memory models, may return
        // false even if the operation was successful
        return true;
    }

    inline bool compare_exchange_strong([[maybe_unused]] int &expected,
                                        [[maybe_unused]] int desired) noexcept
    {
        // intODO
        // less efficient but returns true if the operation was successful
        return true;
    }

  private:
    int _value;
};

} // namespace tenno
