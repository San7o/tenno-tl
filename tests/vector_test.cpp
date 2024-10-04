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

#include <vector>
#include <tenno/vector.hpp>
#include <valfuzz/valfuzz.hpp>

TEST(vector_empty_constructor, "vector empty constructor")
{
    tenno::vector<int> v;
    ASSERT_EQ(v.size(), 0);
    ASSERT_EQ(v.capacity(), 0);
}

TEST(vector_constructor_count_value, "vector constructor with count and value")
{
    tenno::vector<int> v(5, 10);
    ASSERT_EQ(v.size(), 5);
    ASSERT_EQ(v.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v[i], 10);
    }
}

TEST(vector_constructor_count_value_allocator, "vector constructor with count, value and allocator")
{
    tenno::allocator<int> alloc;
    tenno::vector<int> v(5, 10, alloc);
    ASSERT_EQ(v.size(), 5);
    ASSERT_EQ(v.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v[i], 10);
    }
}

TEST(vector_constructor_count, "vector constructor with count")
{
    tenno::vector<int> v(5);
    ASSERT_EQ(v.size(), 5);
    ASSERT_EQ(v.capacity(), 5);
}

TEST(vector_constructor_count_allocator, "vector constructor with count and allocator")
{
    tenno::allocator<int> alloc;
    tenno::vector<int> v(5, alloc);
    ASSERT_EQ(v.size(), 5);
    ASSERT_EQ(v.capacity(), 5);
}

TEST(vector_constructor_copy, "vector constructor with copy")
{
    tenno::vector<int> v1(5, 10);
    tenno::vector<int> v2(v1);
    ASSERT_EQ(v2.size(), 5);
    ASSERT_EQ(v2.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v2[i], 10);
    }
}

TEST(vector_constructor_copy_allocator, "vector constructor with copy and allocator")
{
    tenno::allocator<int> alloc;
    tenno::vector<int> v1(5, 10);
    tenno::vector<int> v2(v1, alloc);
    ASSERT_EQ(v2.size(), 5);
    ASSERT_EQ(v2.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v2[i], 10);
    }
}

TEST(vector_constructor_move, "vector constructor with move")
{
    tenno::vector<int> v1(5, 10);
    tenno::vector<int> v2(std::move(v1));
    ASSERT_EQ(v2.size(), 5);
    ASSERT_EQ(v2.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v2[i], 10);
    }
    ASSERT_EQ(v1.size(), 0);
    ASSERT_EQ(v1.capacity(), 5);
}

TEST(vector_constructor_move_allocator, "vector constructor with move and allocator")
{
    tenno::allocator<int> alloc;
    tenno::vector<int> v1(5, 10);
    tenno::vector<int> v2(std::move(v1), alloc);
    ASSERT_EQ(v2.size(), 5);
    ASSERT_EQ(v2.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v2[i], 10);
    }
    ASSERT_EQ(v1.size(), 0);
    ASSERT_EQ(v1.capacity(), 5);
}

TEST(vector_constructor_initializer_list, "vector constructor with initializer list")
{
    tenno::vector<int> v = {1, 2, 3, 4, 5};
    ASSERT_EQ(v.size(), 5);
    ASSERT_EQ(v.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v[i], i + 1);
    }
}

TEST(vector_constructor_initializer_list_allocator, "vector constructor with initializer list and allocator")
{
    tenno::allocator<int> alloc;
    tenno::vector<int> v = {{1, 2, 3, 4, 5}, alloc};
    ASSERT_EQ(v.size(), 5);
    ASSERT_EQ(v.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v[i], i + 1);
    }
}

TEST(vector_size, "vector size")
{
    tenno::vector<int> v(5, 10);
    ASSERT_EQ(v.size(), 5);
}
