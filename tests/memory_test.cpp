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

#include <tenno/memory.hpp>
#include <valfuzz/valfuzz.hpp>

TEST(make_shared, "tenno::make_shared")
{
    auto sp = tenno::make_shared<int>(5);
    ASSERT_EQ(sp.use_count(), 1);
    ASSERT_EQ(*sp, 5);
}

/*
TEST(make_shared_array_size, "tenno::make_shared [] size")
{
    auto sp = tenno::make_shared<int[5]>(5);
    ASSERT_EQ(sp.use_count(), 1);
}
*/

TEST(make_shared_no_args, "tenno::make_shared no args")
{
    auto sp = tenno::make_shared<int>();
    ASSERT_EQ(sp.use_count(), 1);
    ASSERT_EQ(*sp, 0);
}
