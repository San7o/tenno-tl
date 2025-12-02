// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <tenno/utility.hpp>
#include <valfuzz/valfuzz.hpp>

TEST(move, "tenno::move")
{
  int i = 5;
  int j = tenno::move(i);
  ASSERT_EQ(i, 5);
  ASSERT_EQ(j, 5);
}

TEST(move_constexpr, "constexpr tenno::move")
{
  constexpr int i = 5;
  constexpr int j = tenno::move(i);
  static_assert(i == 5);
  static_assert(j == 5);
}
