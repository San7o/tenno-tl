// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <tenno/expected.hpp>
#include <valfuzz/valfuzz.hpp>

TEST(expected_create, "creating tenno::expected")
{
  [[maybe_unused]] auto e = tenno::expected<int, int>(5);
}

TEST(expected_has_value, "reading tenno::expected.has_value()")
{
  auto e = tenno::expected<int, int>(5);
  ASSERT_EQ(e.has_value(), true);
}

TEST(expected_has_value_false, "reading tenno::expected.has_value() with false")
{
  auto e = tenno::expected<int, int>(tenno::unexpected<int>(5));
  ASSERT_EQ(e.has_value(), false);
}

TEST(expected_bool, "reading tenno::expected as bool")
{
  auto e = tenno::expected<int, int>(5);
  ASSERT_EQ(static_cast<bool>(e), true);
}

TEST(expected_bool_false, "reading tenno::expected as bool with false")
{
  auto e = tenno::expected<int, int>(tenno::unexpected<int>(5));
  e.has_val = false;
  ASSERT_EQ(static_cast<bool>(e), false);
}

TEST(expected_value, "reading tenno::expected::value()")
{
  auto e = tenno::expected<int, int>(5);
  ASSERT_EQ(e.value(), 5);
}

TEST(expected_value_or, "reading tenno::expected::value_or()")
{
  auto e = tenno::expected<int, int>(5);
  ASSERT_EQ(e.value_or(10), 5);
}

TEST(expected_error_or_default,
     "reading tenno::expected::error_or() with default")
{
  auto e = tenno::expected<int, int>(5);
  ASSERT_EQ(e.error_or(10), 10);
}

TEST(expected_constexpr, "create tenno::expected constexpr")
{
  constexpr auto e = tenno::expected<int, int>(5);
  static_assert(e.has_value());
  static_assert(e.value() == 5);
}

TEST(expexted_constexpr_error, "create tenno::expected constexpr with error")
{
  constexpr auto e = tenno::expected<int, int>(tenno::unexpected<int>(5));
  static_assert(!e.has_value());
  static_assert(e.error() == 5);
}
