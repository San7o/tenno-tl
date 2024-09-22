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

TEST(expected_copy, "copying tenno::expected")
{
    auto e = tenno::expected<int, int>(5);
    auto e2 = e;
    ASSERT_EQ(e2.value(), 5);
}

TEST(expected_move, "moving tenno::expected")
{
    auto e = tenno::expected<int, int>(5);
    auto e2 = std::move(e);
    ASSERT_EQ(e2.value(), 5);
}

TEST(expected_copy_assign, "copy assigning tenno::expected")
{
    auto e = tenno::expected<int, int>(5);
    auto e2 = tenno::expected<int, int>(10);
    e2 = e;
    ASSERT_EQ(e2.value(), 5);
}

TEST(expected_move_assign, "move assigning tenno::expected")
{
    auto e = tenno::expected<int, int>(5);
    auto e2 = tenno::expected<int, int>(10);
    e2 = std::move(e);
    ASSERT_EQ(e2.value(), 5);
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
