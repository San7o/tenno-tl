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

namespace tenno
{

/**
 * @brief A simple mutex implementation
 */
class mutex
{
  public:
    mutex() = default;
    ~mutex() = default;

    /**
     * @brief Lock the mutex
     */
    inline void lock() noexcept
    {
        int expected = 0;

#if defined(__GNUC__) || !defined(__clang__)
        while (!__atomic_compare_exchange_n(
            &this->is_locked, /* Address of the mutex variable */
            &expected,        /* address of the expected value */
            1,                /* desired value */
            0,                /* strong compare-and-swap (retry on failure) */
            __ATOMIC_SEQ_CST, /* memory order */
            __ATOMIC_SEQ_CST))
        {
            expected = 0;
        }
#elif __x86_64__
        while (true)
        {
            asm volatile("movl %1, %%eax\n\t" /* move expexted value to eax */
                         "lock cmpxchgl %2, %0\n\t" /* compare and exchange */
                         : "+m"(this->is_locked)    /* %0: output operand */
                         : "r"(expected),  /* %1: expected value (input) */
                           "r"(1)          /* %2: new value to set */
                         : "eax", "memory" /* clobbered registers */
            );

            if (this->is_locked == 1)
            {
                return;
            }

            asm volatile("pause");
        }
#else
#error "Unsupported platform"
#endif
    }

    /**
     * @brief Try to lock the mutex
     *
     * @return true if the mutex was locked, false otherwise
     */
    inline bool try_lock() noexcept
    {
        int expected = 0;

#if defined(__GNUC__) || !defined(__clang__)
        return __atomic_compare_exchange_n(
            &this->is_locked, /* Address of the mutex variable */
            &expected,        /* address of the expected value */
            1,                /* desired value */
            0,                /* strong compare-and-swap (retry on failure) */
            __ATOMIC_SEQ_CST, /* memory order */
            __ATOMIC_SEQ_CST);
#elif __x86_64__
        asm volatile("movl %1, %%eax\n\t"       /* move expexted value to eax */
                     "lock cmpxchgl %2, %0\n\t" /* compare and exchange */
                     : "+m"(this->is_locked)    /* %0: output operand */
                     : "r"(expected),           /* %1: expected value (input) */
                       "r"(1)                   /* %2: new value to set */
                     : "eax", "memory"          /* clobbered registers */
        );

        return (bool) this->is_locked;

#else
#error "Unsupported platform"
#endif
    }

    /**
     * @brief Unlock the mutex
     */
    inline void unlock() noexcept
    {
#if defined(__GNUC__) || !defined(__clang__)
        __atomic_store_n(&this->is_locked, 0, __ATOMIC_SEQ_CST);
#elif __x86_64__
        asm volatile("movl $0, %0\n\t"       /* set the state to 0 */
                     : "+m"(this->is_locked) /* %0: output operand */
                     :                       /* no input operands */
                     : "memory"              /* clobbered registers */
        );
#else
#error "Unsupported platform"
#endif
    }

#ifndef TENNO_DEBUG
  private:
#endif
    volatile int is_locked = 0;
};

/**
 * @brief A simple lock guard implementation
 *
 * @tparam M The type of the mutex to guard
 *
 * Locks the mutex in the constructor and unlocks it in the destructor.
 */
template <typename M> class lock_guard
{
  public:
    lock_guard(M &m) : _m(m)
    {
        this->_m.lock();
    }

    ~lock_guard()
    {
        this->_m.unlock();
    }

    void operator=(const lock_guard &) = delete;

  private:
    M &_m;
};

} // namespace tenno
