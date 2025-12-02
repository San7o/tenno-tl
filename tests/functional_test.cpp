// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <tenno/functional.hpp>
#include <tenno/utility.hpp>
#include <valfuzz/valfuzz.hpp>

TEST(reference_wrapper, "reference_wrapper")
{
  int x = 10;
  tenno::reference_wrapper<int> rw(tenno::move(x));
  ASSERT_EQ(x, 10);
  ASSERT_EQ(rw.ref(), 10);
  x = 20;
  ASSERT_EQ(rw.ref(), 20);
}

TEST(reference_wrapper_mutate, "reference_wrapper mutate")
{
  int x = 10;
  tenno::reference_wrapper<int> rw(tenno::move(x));
  ASSERT_EQ(x, 10);
  ASSERT_EQ(rw.ref(), 10);
  rw.ref() = 20;
  ASSERT_EQ(x, 20);
  ASSERT_EQ(rw.ref(), 20);
  int &y = rw;
  y = 30;
  ASSERT_EQ(x, 30);
  ASSERT_EQ(rw.ref(), 30);
}

TEST(reference_wrapper_copy, "reference_wrapper copy")
{
  int x = 10;
  tenno::reference_wrapper<int> rw(tenno::move(x));
  tenno::reference_wrapper<int> rw2(rw);
  ASSERT_EQ(rw.ref(), 10);
  ASSERT_EQ(rw2.ref(), 10);
  x = 20;
  ASSERT_EQ(rw.ref(), 20);
  ASSERT_EQ(rw2.ref(), 20);
}

TEST(reference_wrapper_copy_assignment, "reference_wrapper copy assignment")
{
  int x = 10;
  tenno::reference_wrapper<int> rw(tenno::move(x));
  tenno::reference_wrapper<int> rw2;
  rw2 = rw;
  ASSERT_EQ(rw.ref(), 10);
  ASSERT_EQ(rw2.ref(), 10);
  x = 20;
  ASSERT_EQ(rw.ref(), 20);
  ASSERT_EQ(rw2.ref(), 20);
}
