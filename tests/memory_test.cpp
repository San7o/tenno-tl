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

#include <tenno/array.hpp>
#include <tenno/vector.hpp>
#include <tenno/memory.hpp>
#include <valfuzz/valfuzz.hpp>

TEST(make_shared, "tenno::make_shared")
{
    auto sp = tenno::make_shared<int>(5);
    ASSERT_EQ(sp.use_count(), 1);
    ASSERT_EQ(*sp, 5);
}

TEST(make_shared_array, "tenno::make_shared T[]")
{
    auto sp = tenno::make_shared<int[10]>(10);
    ASSERT_EQ(sp.use_count(), 1);
}

TEST(make_shared_tenno_array, "tenno::make_shared tenno::array")
{
    tenno::array<int, 10> arr = {1, 2, 3, 4};
    auto sp = tenno::make_shared<tenno::array<int, 10>>(std::move(arr));
    ASSERT_EQ(sp.use_count(), 1);
}

TEST(make_shared_no_args, "tenno::make_shared no args")
{
    auto sp = tenno::make_shared<int>();
    ASSERT_EQ(sp.use_count(), 1);
    ASSERT_EQ(*sp, 0);
}

TEST(make_shared_vector, "tenno::make_shared tenno::vector")
{
    tenno::vector<int> vec = {1, 2, 3, 4};
    auto sp = tenno::make_shared<tenno::vector<int>>(std::move(vec));
    ASSERT_EQ(sp.use_count(), 1);
}

TEST(allocate_shared, "tenno::allocate_shared")
{
    std::allocator<int> alloc;
    auto sp = tenno::allocate_shared<int>(alloc);
    ASSERT_EQ(sp.use_count(), 1);
    ASSERT_EQ(*sp, 0);
}

TEST(allocate_shared_args, "tenno::allocate_shared with args")
{
    std::allocator<int> alloc;
    auto sp = tenno::allocate_shared<int>(alloc, 5);
    ASSERT_EQ(sp.use_count(), 1);
    ASSERT_EQ(*sp, 5);
}

TEST(make_unique, "tenno::make_unique")
{
    auto up = tenno::make_unique<int>(5);
    ASSERT_EQ(*up, 5);
}

/*
TEST(allocate_shared_array, "tenno::allocate_shared array")
{
    std::allocator<int> alloc;
    auto sp = tenno::allocate_shared<int[10]>(alloc, 10);
    for (int i = 0; i < 10; i++)
    {
        sp[i] = i;
    }
    ASSERT_EQ(sp.use_count(), 1);
}
*/
