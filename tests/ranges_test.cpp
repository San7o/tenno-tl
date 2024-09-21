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

#include <valfuzz/valfuzz.hpp>
#include <tenno/ranges.hpp>

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
}
