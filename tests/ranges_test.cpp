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
