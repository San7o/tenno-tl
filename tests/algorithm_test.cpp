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
#include <tenno/array.hpp>
#include <valfuzz/valfuzz.hpp>

TEST(algorithm_copy, "copying tenno::array")
{
    tenno::array<int, 5> arr{1, 2, 3, 4, 5};
    tenno::array<int, 5> arr2;
    tenno::copy(arr.begin(), arr.end(), arr2.data);
    ASSERT_EQ(arr2.data[0], 1);
    ASSERT_EQ(arr2.data[1], 2);
    ASSERT_EQ(arr2.data[2], 3);
    ASSERT_EQ(arr2.data[3], 4);
    ASSERT_EQ(arr2.data[4], 5);
}
