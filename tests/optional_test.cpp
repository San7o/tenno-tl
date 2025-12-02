// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <tenno/optional.hpp>
#include <valfuzz/valfuzz.hpp>

TEST(optional_constructor, "tenno::optional()")
{
  tenno::optional<int> opt;
  ASSERT(!opt.has_value());
}

TEST(optional_constructor_item, "tenno::optional(int)")
{
  tenno::optional<int> opt(42);
  ASSERT(opt.has_value());
  ASSERT_EQ(*opt, 42);
}

TEST(optional_has_value, "tenno::optional.has_value()")
{
  tenno::optional<int> opt(42);
  ASSERT(opt.has_value());
}

TEST(optional_value, "tenno::optional.value()")
{
  tenno::optional<int> opt(42);
  ASSERT_EQ(opt.value(), 42);
}

TEST(optional_value_or, "tenno::optional.value_or() value")
{
  tenno::optional<int> opt(42);
  ASSERT_EQ(opt.value_or(0), 42);
}

TEST(optional_value_or_or, "tenno::optional.value_or() or")
{
  tenno::optional<int> opt;
  ASSERT_EQ(opt.value_or(0), 0);
}

TEST(optional_swap, "tenno::optional.swap")
{
  tenno::optional<int> opt1(10);
  tenno::optional<int> opt2(11);
  opt1.swap(opt2);
  ASSERT_EQ(opt1.value(), 11);
  ASSERT_EQ(opt2.value(), 10);
}

TEST(optional_assign, "tenno::optional =")
{
  tenno::optional<int> opt;
  opt = 42;
  ASSERT_EQ(opt.value(), 42);
}

TEST(optional_reset, "tenno::optional.reset()")
{
  tenno::optional<int> opt(10);
  ASSERT(opt.has_value());
  opt.reset();
  ASSERT(!opt.has_value());
}

template <typename... Args> struct optional_emplace_test_struct
{
  int a;
  optional_emplace_test_struct()
  {
  }
  optional_emplace_test_struct(Args... args) : a(args...)
  {
  }
};

TEST(optional_emplace, "tenno::optional.emplace()")
{
  tenno::optional<optional_emplace_test_struct<int>> opt;
  opt.emplace(10);
  ASSERT(opt.has_value());
  ASSERT(opt.value().a == 10);
}

#if __cplusplus >= 202002L // C++20
TEST(optional_constexpr_constructor, "constexpr tenno::optional()")
{
  constexpr tenno::optional<int> o = tenno::optional<int>();
  static_assert(!o.has_value());
}

TEST(optional_constexpr_constructor_full, "constexpr tenno::optional(int)")
{
  constexpr tenno::optional<int> o(10);
  static_assert(o.has_value());
  static_assert(o.value() == 10);
}

TEST(optional_constexpr_value_or, "constexpr tenno::optional.value_or()")
{
  constexpr tenno::optional<int> o(10);
  static_assert(o.value_or(20) == 10);
}
#endif
