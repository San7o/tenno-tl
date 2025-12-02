// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <tenno/algorithm.hpp>
#include <tenno/ranges.hpp>
#include <valfuzz/valfuzz.hpp>

TEST(range_create, "creating tenno::range")
{
  [[maybe_unused]] auto r = tenno::range<int>(0, 5);
}

TEST(range_size, "reading tenno::range.size()")
{
  auto r = tenno::range<int>(0, 5);
  ASSERT_EQ(r.size(), 5);
}

TEST(range_begin, "reading tenno::range.begin()")
{
  auto r = tenno::range<int>(0, 5);
  ASSERT_EQ(*r.begin(), 0);
}

TEST(range_end, "reading tenno::range.end()")
{
  auto r = tenno::range<int>(0, 5);
  ASSERT_EQ(*r.end(), 5);
}

TEST(range_iterate, "iterating over tenno::range")
{
  auto r = tenno::range<int>(0, 5);
  auto it = r.begin();
  for (int i = 0; i < 5; ++i)
  {
    ASSERT_EQ(*it, i);
    ++it;
  }

  int sum = 0;
  for (auto i : r)
  {
    sum += i;
  }
  ASSERT_EQ(sum, 10);
}

TEST(range_constexpr, "tenno::range is constexpr")
{
  constexpr auto r = tenno::range<int>(0, 5);
  static_assert(r.size() == 5);
  static_assert(*r.begin() == 0);
  static_assert(*r.end() == 5);
}

TEST(range_constexpr_iterate, "tenno::range::Iterator is constexpr")
{
  constexpr auto r = tenno::range<int>(0, 5);
  constexpr auto sum = tenno::accumulate(r.begin(), r.end(), 0);
  static_assert(sum == 10);
}

TEST(range_short_constructor, "short constructor for tenno::range")
{
  auto r = tenno::range<int>(5);
  ASSERT_EQ(r.size(), 5);
  ASSERT_EQ(*r.begin(), 0);
  ASSERT_EQ(*r.end(), 5);
}
