// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <tenno/algorithm.hpp>
#include <tenno/array.hpp>
#include <valfuzz/valfuzz.hpp>

TEST(algorithm_copy, "copying tenno::array")
{
  tenno::array<int, 5> arr{1, 2, 3, 4, 5};
  tenno::array<int, 5> arr2;
  tenno::copy(arr.begin(), arr.end(), arr2.data());
  ASSERT_EQ(arr2.data()[0], 1);
  ASSERT_EQ(arr2.data()[1], 2);
  ASSERT_EQ(arr2.data()[2], 3);
  ASSERT_EQ(arr2.data()[3], 4);
  ASSERT_EQ(arr2.data()[4], 5);
}

TEST(algorithm_for_each, "iterating over tenno::array with for_each")
{
  tenno::array<int, 5> arr{1, 2, 3, 4, 5};
  int sum = 0;
  tenno::for_each(arr.begin(), arr.end(), [&sum](int i) { sum += i; });
  ASSERT_EQ(sum, 15);
}

#if __cplusplus >= 202002L // C++20
TEST(algorithm_for_each_constexpr,
     "iterating over tenno::array with for_each constexpr")
{
  constexpr tenno::array<int, 5> arr{1, 2, 3, 4, 5};
  constexpr const auto sum = [arr]() constexpr
  {
    auto tmp = 0;
    tenno::for_each(arr.cbegin(), arr.cend(), [&tmp](int i) { tmp += i; });
    return tmp;
  }();
  static_assert(sum == 15);
}
#endif

TEST(algorithm_accumulate, "accumulating tenno::array")
{
  tenno::array<int, 5> arr{1, 2, 3, 4, 5};
  auto sum = tenno::accumulate(arr.cbegin(), arr.cend(), 0);
  ASSERT_EQ(sum, 15);
}

#if __cplusplus >= 202002L // C++20
TEST(algorithm_accumulate_constexpr, "accumulating tenno::array constexpr")
{
  constexpr tenno::array<int, 5> arr{1, 2, 3, 4, 5};
  constexpr auto sum = tenno::accumulate(arr.cbegin(), arr.cend(), 0);
  static_assert(sum == 15);
}
#endif

TEST(algorithm_swap, "tenno::swap")
{
  int a = 1;
  int b = 2;
  tenno::swap(a, b);
  ASSERT(a == 2);
  ASSERT(b == 1);
}
