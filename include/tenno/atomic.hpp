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
        // TODO: use mutexes I guess?
        this->_value = desired;
    }

    inline T load() const noexcept
    {
        // TODO: use mutexes
        return this->_value;
    }

    operator T() const noexcept
    {
        return this->load();
    }

    inline T exchange(T desired) noexcept
    {
        // read-modify-write operation (CAS)
        // TODO
        return desired;
    }

    inline bool compare_exchange_weak(T &expected, T desired) noexcept
    {
        // TODO
        // more efficient and used in weak memory models, may return
        // false even if the operation was successful
        return true;
    }

    inline bool compare_exchange_strong(T &expected, T desired) noexcept
    {
        // TODO
        // less efficient but returns true if the operation was successful
        return true;
    }

    // TODO
    // - wait
    // - notify_one
    // - notify_all

  private:
    T _value;
};

/* general pointer */
template <typename U> class atomic<U *>
{
    // TODO
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
    const bool is_always_lock_free = true; // ig

    atomic() noexcept = default;

    inline bool is_lock_free() const noexcept
    {
        return this->is_always_lock_free;
    }

    // TODO

  private:
    int _value;
};

} // namespace tenno
