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
 * copies or substantial portions of the Snooftware.
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
