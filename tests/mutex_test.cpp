// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

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
