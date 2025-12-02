// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <tenno/thread.hpp>
#include <valfuzz/valfuzz.hpp>

TEST(jthread_create, "tenno::jthread creation")
{
  int i = 0;
  tenno::jthread t1([&i]() { i = 10; });
  ASSERT(t1.joinable());
}

TEST(jthread_swap, "tenno::jthread swap")
{
  int i = 0;
  tenno::jthread t1([&i]() { i = 10; });
  int j = 0;
  tenno::jthread t2([&j]() { j = 10; });
  std::thread::id id1 = t1.id;
  std::thread::id id2 = t2.id;
  t1.swap(t2);
  ASSERT(t1.joinable());
  ASSERT(t2.joinable());
  ASSERT(t1.id == id2);
  ASSERT(t2.id == id1);
}
