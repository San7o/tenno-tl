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

#include <tenno/mutex.hpp>
#include <valfuzz/valfuzz.hpp>

TEST(mutex_create, "Creating a mutex")
{
    tenno::mutex m;
}

TEST(mutex_lock, "Locking a mutex")
{
    tenno::mutex m;
    m.lock();
    ASSERT(m.is_locked);
}

TEST(mutex_unlock, "Unlocking a mutex")
{
    tenno::mutex m;
    m.lock();
    ASSERT(m.is_locked);
    m.unlock();
    ASSERT(!m.is_locked);
}

TEST(mutex_try_lock, "Trying to lock a mutex")
{
    tenno::mutex m;
    ASSERT(m.try_lock());
    ASSERT(m.is_locked);
    ASSERT(!m.try_lock());
}

// lock guard

TEST(lock_guard_create, "Creating a lock guard")
{
    tenno::mutex m;
    tenno::lock_guard<tenno::mutex> lg(m);
}

TEST(lock_guard_lock, "Locking a mutex with a lock guard")
{
    tenno::mutex m;
    tenno::lock_guard<tenno::mutex> lg(m);
    ASSERT(m.is_locked);
}

TEST(lock_guard_unlock, "Unlocking a mutex with a lock guard")
{
    tenno::mutex m;
    {
        tenno::lock_guard<tenno::mutex> lg(m);
    }
    ASSERT(!m.is_locked);
}
