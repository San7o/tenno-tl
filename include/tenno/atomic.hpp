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

/**
 * @brief The atomic class template provides operations on atomic types.
 *
 * @tparam T The type of the atomic object.
 */
template <typename T> class atomic;

/* general case */
template <typename T> class atomic
{
  public:
    /**
     * @brief The type of the atomic object.
     */
    using value_type = T;
    /**
     * @brief Whether the atomic object is always lock free.
     */
    const bool is_always_lock_free = false;

    atomic() noexcept = default;
    ~atomic() noexcept = default;
    atomic(const atomic &) = delete;
    atomic &operator=(const atomic &) = delete;
    atomic &operator=(const atomic &) volatile = delete;

    /**
     * @brief Check if the atomic object is lock free.
     *
     * @return true if the atomic object is lock free, false otherwise.
     */
    inline bool is_lock_free() const noexcept
    {
        return this->is_always_lock_free;
    }

    /**
     * @brief Store a value in the atomic object.
     *
     * @param desired The value to store.
     */
    inline void store(T desired) noexcept
    {
        tenno::lock_guard<tenno::mutex> lock(this->_mutex);
        this->_value = desired;
    }

    /**
     * @brief Load the value from the atomic object.
     *
     * @return T The value stored in the atomic object.
     */
    inline T load() noexcept
    {
        tenno::lock_guard<tenno::mutex> lock(this->_mutex);
        return this->_value;
    }

    /**
     * @brief Implicit conversion to the value stored in the atomic object.
     *
     * @return T The value stored in the atomic object.
     */
    operator T() noexcept
    {
        return this->load();
    }

    /**
     * @brief Exchange the value stored in the atomic object with a new value.
     *
     * @param desired The new value to store in the atomic object.
     * @return T The old value stored in the atomic object.
     */
    inline T exchange(T desired) noexcept
    {
        tenno::lock_guard<tenno::mutex> lock(this->_mutex);
        T old = this->_value;
        this->_value = desired;
        return old;
    }

    /**
     * @brief Compare and exchange the value of the atomic object
     * @param expected The expected value
     * @param desired The desired value
     * @return true if the exchange was successful, false otherwise
     */
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

    /**
     * @brief Compare and exchange the value of the atomic object
     * @param expected The expected value
     * @param desired The desired value
     * @return true
     * @note This function is always successful.
     */
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
    using value_type = U *;
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

    inline void store(U *desired) noexcept
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

    inline U exchange(U *desired) noexcept
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
    inline bool compare_exchange_weak(const U *expected, U *desired) noexcept
    {
        tenno::lock_guard<tenno::mutex> lock(this->_mutex);
        if (this->_value != nullptr && *this->_value == *expected)
        {
            *this->_value = *desired;
            return true;
        }
        return false;
    }

    inline bool compare_exchange_strong(const U *expected, U *desired) noexcept
    {
        tenno::lock_guard<tenno::mutex> lock(this->_mutex);
        if (this->_value != nullptr && *this->_value == *expected)
        {
            *this->_value = *desired;
            return true;
        }
        return false;
    }

  private:
    U *_value;
    tenno::mutex _mutex;
};

/* int specialization */
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
        asm volatile("lock xchg %0, %1" : "+m"(this->_value), "+r"(desired));
    }

    inline int load() noexcept
    {
        int out = 0;
        asm volatile("lock add %1, %0" : "+m"(out), "+r"(this->_value));
        return out;
    }

    operator int() noexcept
    {
        return this->load();
    }

    inline int exchange(int desired) noexcept
    {
        asm volatile("lock xchg %0, %1" : "+m"(this->_value), "+r"(desired));
        return desired;
    }

    inline bool compare_exchange_weak(int &expected, int desired) noexcept
    {
        bool success;
        asm volatile("lock cmpxchg %[desired], %[target]\n\t"
                     "sete %[success]\n\t"
                     : [success] "=r"(success), "=m"(this->_value)
                     : [target] "m"(this->_value),
                       "a"(expected), [desired] "r"(desired)
                     : "memory", "cc");
        return success;
    }

    inline bool compare_exchange_strong(int &expected, int desired) noexcept
    {
        bool success = false;
        while (!success)
        {
            asm volatile("lock cmpxchg %[desired], %[target]\n\t"
                         "sete %[success]\n\t"
                         : [success] "=r"(success), "=m"(this->_value)
                         : [target] "m"(this->_value),
                           "a"(expected), [desired] "r"(desired)
                         : "memory", "cc");
        }
        return true;
    }

  private:
    int _value;
};

/* char specialization */

template <> class atomic<char>
{
  public:
    using value_type = char;
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

    inline void store(char desired) noexcept
    {
        asm volatile("lock xchg %0, %1" : "+m"(this->_value), "+r"(desired));
    }

    inline char load() noexcept
    {
        char out = 0;
        asm volatile("lock add %1, %0" : "+m"(out), "+r"(this->_value));
        return out;
    }

    operator char() noexcept
    {
        return this->load();
    }

    inline char exchange(char desired) noexcept
    {
        asm volatile("lock xchg %0, %1" : "+m"(this->_value), "+r"(desired));
        return desired;
    }

    inline bool compare_exchange_weak(char &expected, char desired) noexcept
    {
        bool success;
        asm volatile("lock cmpxchg %[desired], %[target]\n\t"
                     "sete %[success]\n\t"
                     : [success] "=r"(success), "=m"(this->_value)
                     : [target] "m"(this->_value),
                       "a"(expected), [desired] "r"(desired)
                     : "memory", "cc");
        return success;
    }

    inline bool compare_exchange_strong(char &expected, char desired) noexcept
    {
        bool success = false;
        while (!success)
        {
            asm volatile("lock cmpxchg %[desired], %[target]\n\t"
                         "sete %[success]\n\t"
                         : [success] "=r"(success), "=m"(this->_value)
                         : [target] "m"(this->_value),
                           "a"(expected), [desired] "r"(desired)
                         : "memory", "cc");
        }
        return true;
    }

  private:
    char _value;
};

/* long specialization */

template <> class atomic<long>
{
  public:
    using value_type = long;
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

    inline void store(long desired) noexcept
    {
        asm volatile("lock xchg %0, %1" : "+m"(this->_value), "+r"(desired));
    }

    inline long load() noexcept
    {
        long out = 0;
        asm volatile("lock add %1, %0" : "+m"(out), "+r"(this->_value));
        return out;
    }

    operator long() noexcept
    {
        return this->load();
    }

    inline long exchange(long desired) noexcept
    {
        asm volatile("lock xchg %0, %1" : "+m"(this->_value), "+r"(desired));
        return desired;
    }

    inline bool compare_exchange_weak(long &expected, long desired) noexcept
    {
        bool success;
        asm volatile("lock cmpxchg %[desired], %[target]\n\t"
                     "sete %[success]\n\t"
                     : [success] "=r"(success), "=m"(this->_value)
                     : [target] "m"(this->_value),
                       "a"(expected), [desired] "r"(desired)
                     : "memory", "cc");
        return success;
    }

    inline bool compare_exchange_strong(long &expected, long desired) noexcept
    {
        bool success = false;
        while (!success)
        {
            asm volatile("lock cmpxchg %[desired], %[target]\n\t"
                         "sete %[success]\n\t"
                         : [success] "=r"(success), "=m"(this->_value)
                         : [target] "m"(this->_value),
                           "a"(expected), [desired] "r"(desired)
                         : "memory", "cc");
        }
        return true;
    }

  private:
    long _value;
};

} // namespace tenno
