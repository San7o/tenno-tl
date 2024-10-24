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

#include <tenno/ranges.hpp>
#include <tenno/vector.hpp>
#include <valfuzz/valfuzz.hpp>
#include <vector>

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
        ASSERT_EQ(v[i].value(), 10);
    }
}

TEST(vector_constructor_count_value_allocator,
     "vector constructor with count, value and allocator")
{
    tenno::allocator<int> alloc;
    tenno::vector<int> v(5, 10, alloc);
    ASSERT_EQ(v.size(), 5);
    ASSERT_EQ(v.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v[i].value(), 10);
    }
}

TEST(vector_constructor_count, "vector constructor with count")
{
    tenno::vector<int> v(5);
    ASSERT_EQ(v.size(), 5);
    ASSERT_EQ(v.capacity(), 5);
}

TEST(vector_constructor_count_allocator,
     "vector constructor with count and allocator")
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
        ASSERT_EQ(v2[i].value(), 10);
    }
}

TEST(vector_constructor_copy_allocator,
     "vector constructor with copy and allocator")
{
    tenno::allocator<int> alloc;
    tenno::vector<int> v1(5, 10);
    tenno::vector<int> v2(v1, alloc);
    ASSERT_EQ(v2.size(), 5);
    ASSERT_EQ(v2.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v2[i].value(), 10);
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
        ASSERT_EQ(v2[i].value(), 10);
    }
    ASSERT_EQ(v1.size(), 0);
    ASSERT_EQ(v1.capacity(), 5);
}

TEST(vector_constructor_move_allocator,
     "vector constructor with move and allocator")
{
    tenno::allocator<int> alloc;
    tenno::vector<int> v1(5, 10);
    tenno::vector<int> v2(std::move(v1), alloc);
    ASSERT_EQ(v2.size(), 5);
    ASSERT_EQ(v2.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v2[i].value(), 10);
    }
    ASSERT_EQ(v1.size(), 0);
    ASSERT_EQ(v1.capacity(), 5);
}

TEST(vector_constructor_initializer_list,
     "vector constructor with initializer list")
{
    tenno::vector<int> v = {1, 2, 3, 4, 5};
    ASSERT_EQ(v.size(), 5);
    ASSERT_EQ(v.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v[i].value(), i + 1);
    }
}

TEST(vector_constructor_initializer_list_allocator,
     "vector constructor with initializer list and allocator")
{
    tenno::allocator<int> alloc;
    tenno::vector<int> v = {{1, 2, 3, 4, 5}, alloc};
    ASSERT_EQ(v.size(), 5);
    ASSERT_EQ(v.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v[i].value(), i + 1);
    }
}

TEST(vector_copy_assignment, "vector copy assignment")
{
    tenno::vector<int> v1(5, 10);
    tenno::vector<int> v2;
    v2 = v1;
    ASSERT_EQ(v2.size(), 5);
    ASSERT_EQ(v2.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v2[i].value(), 10);
    }
}

TEST(vector_move_assignment, "vector move assignment")
{
    tenno::vector<int> v1(5, 10);
    tenno::vector<int> v2;
    v2 = std::move(v1);
    ASSERT_EQ(v2.size(), 5);
    ASSERT_EQ(v2.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v2[i].value(), 10);
    }
    ASSERT_EQ(v1.size(), 0);
    ASSERT_EQ(v1.capacity(), 0);
}

TEST(vector_initializer_list_assignment, "vector initializer list assignment")
{
    tenno::vector<int> v;
    v = {1, 2, 3, 4, 5};
    ASSERT_EQ(v.size(), 5);
    ASSERT_EQ(v.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v[i].value(), i + 1);
    }
}

TEST(vector_assign, "vector assign")
{
    tenno::vector<int> v(5, 10);
    v.assign(3, 20);
    ASSERT_EQ(v.size(), 5);
    ASSERT_EQ(v.capacity(), 5);
    for (int i = 0; i < 3; i++)
    {
        ASSERT_EQ(v[i].value(), 20);
    }
}

/*
TEST(vector_assign_iterator, "vector assign iterator")
{
    tenno::vector<int> v(5, 10);
    std::vector<int> v2 = {1, 2, 3, 4, 5};
    v.assign(v2.begin(), v2.end());
    ASSERT_EQ(v.size(), 5);
    ASSERT_EQ(v.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v[i].value(), i + 1);
    }
}
*/

TEST(vector_assign_initializer_list, "vector assign initializer list")
{
    tenno::vector<int> v(5, 10);
    v.assign({1, 2, 3, 4, 5});
    ASSERT_EQ(v.size(), 5);
    ASSERT_EQ(v.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v[i].value(), i + 1);
    }
}

TEST(vector_assign_range, "vector assign range")
{
    tenno::vector<int> v(2, 10);
    tenno::range<int> r(1, 6);
    v.assign_range(r);
    ASSERT_EQ(v.size(), 5);
    ASSERT_EQ(v.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v[i].value(), i + 1);
    }
}

TEST(vector_get_allocator, "vector get allocator")
{
    tenno::vector<int> v;
    tenno::allocator<int> alloc = v.get_allocator();
    ASSERT_EQ(alloc, tenno::allocator<int>());
}

TEST(vector_at, "vector at")
{
    tenno::vector<int> v(5, 10);
    ASSERT_EQ(v.at(0).value(), 10);
    ASSERT_EQ(v.at(1).value(), 10);
    ASSERT_EQ(v.at(2).value(), 10);
    ASSERT_EQ(v.at(3).value(), 10);
    ASSERT_EQ(v.at(4).value(), 10);
    ASSERT_EQ(v.at(5).has_value(), false);
}

TEST(vector_at_mutate, "vector at mutate")
{
    tenno::vector<int> v(5, 10);
    v.at(0).value().ref() = 20;
    v.at(1).value().ref() = 30;
    v.at(2).value().ref() = 40;
    v.at(3).value().ref() = 50;
    v.at(4).value().ref() = 60;
    ASSERT_EQ(v.at(0).value(), 20);
    ASSERT_EQ(v.at(1).value(), 30);
    ASSERT_EQ(v.at(2).value(), 40);
    ASSERT_EQ(v.at(3).value(), 50);
    ASSERT_EQ(v.at(4).value(), 60);
}

TEST(vector_operator_brackets, "vector operator brackets")
{
    tenno::vector<int> v(5, 10);
    ASSERT_EQ(v[0].value(), 10);
    ASSERT_EQ(v[1].value(), 10);
    ASSERT_EQ(v[2].value(), 10);
    ASSERT_EQ(v[3].value(), 10);
    ASSERT_EQ(v[4].value(), 10);
    ASSERT_EQ(v[5].has_value(), false);
}

TEST(vector_operator_brackets_mutate, "vector operator brackets mutate")
{
    tenno::vector<int> v(5, 10);
    v[0].value().ref() = 20;
    v[1].value().ref() = 30;
    v[2].value().ref() = 40;
    v[3].value().ref() = 50;
    v[4].value().ref() = 60;
    ASSERT_EQ(v[0].value(), 20);
    ASSERT_EQ(v[1].value(), 30);
    ASSERT_EQ(v[2].value(), 40);
    ASSERT_EQ(v[3].value(), 50);
    ASSERT_EQ(v[4].value(), 60);
}

TEST(vector_front, "vector front")
{
    tenno::vector<int> v(5, 10);
    ASSERT_EQ(v.front().value(), 10);
    tenno::vector<int> v2 = {1, 2, 3, 4, 5};
    ASSERT_EQ(v2.front().value(), 1);
}

TEST(vector_back, "vector back")
{
    tenno::vector<int> v(5, 10);
    ASSERT_EQ(v.back().value(), 10);
    tenno::vector<int> v2 = {1, 2, 3, 4, 5};
    ASSERT_EQ(v2.back().value(), 5);
}

TEST(vector_data, "vector data")
{
    tenno::vector<int> v(5, 10);
    auto exp = v.data();
    ASSERT_EQ(exp.has_value(), true);
    int *data = exp.value();
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(data[i], 10);
    }
}

TEST(vector_iterator, "vector iterator")
{
    tenno::vector<int> v(5, 10);
    int i = 0;
    for (auto it = v.begin(); it != v.end(); it++)
    {
        ASSERT_EQ((*it).value(), 10);
        i++;
    }
    ASSERT_EQ(i, 5);
}

TEST(vector_iterator_get, "vector iterator get")
{
    tenno::vector<int> v = {1, 2, 3, 4, 5};
    int i = 0;
    for (auto it = v.begin(); it != v.end(); it++)
    {
        ASSERT_EQ(it.get().value(), ++i);
    }
    ASSERT_EQ(i, 5);
}

TEST(vector_iterator_mutate, "vector iterator mutate")
{
    tenno::vector<int> v = {1, 2, 3, 4, 5};
    for (auto it = v.begin(); it != v.end(); it++)
    {
        it.get().value().ref() = 5;
    }
    for (auto it = v.begin(); it != v.end(); it++)
    {
        ASSERT_EQ(it.get().value(), 5);
    }
}

TEST(vector_reverse_iterator, "vector reverse iterator")
{
    tenno::vector<int> v = {1, 2, 3, 4, 5};
    int i = 5;
    for (auto it = v.rbegin(); it != v.rend(); it++)
    {
        ASSERT_EQ((*it).value(), i--);
    }
    ASSERT_EQ(i, 0);
}

TEST(vector_empty, "vector empty")
{
    tenno::vector<int> v;
    ASSERT_EQ(v.empty(), true);
    tenno::vector<int> v2(5, 10);
    ASSERT_EQ(v2.empty(), false);
}

TEST(vector_size, "vector size")
{
    tenno::vector<int> v(5, 10);
    ASSERT_EQ(v.size(), 5);
    tenno::vector<int> v2;
    ASSERT_EQ(v2.size(), 0);
    tenno::vector<int> v3(10);
    ASSERT_EQ(v3.size(), 10);
}

TEST(vector_max_size, "vector max size")
{
    tenno::vector<int> v;
    ASSERT_EQ(v.max_size(), (tenno::size) -1);
}

TEST(vector_reserve, "vector reserve")
{
    tenno::vector<int> v(5, 10);
    v.reserve(10);
    ASSERT_EQ(v.capacity(), 10);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v[i].value(), 10);
    }
    v.reserve(3);
    ASSERT_EQ(v.capacity(), 10);
    v.reserve(20);
    ASSERT_EQ(v.capacity(), 20);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v[i].value(), 10);
    }
}

TEST(vector_capacity, "vector capacity")
{
    tenno::vector<int> v(5, 10);
    ASSERT_EQ(v.capacity(), 5);
    tenno::vector<int> v2;
    ASSERT_EQ(v2.capacity(), 0);
    tenno::vector<int> v3(10);
    ASSERT_EQ(v3.capacity(), 10);
}

TEST(vector_shrink_to_fit, "vector shrink to fit")
{
    tenno::vector<int> v(5, 10);
    v.reserve(10);
    v.shrink_to_fit();
    ASSERT_EQ(v.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v[i].value(), 10);
    }
}

TEST(vector_clear, "vector clear")
{
    tenno::vector<int> v(5, 10);
    ASSERT(v.size() == 5);
    ASSERT(v.capacity() == 5);
    v.clear();
    ASSERT(v.size() == 0);
    ASSERT(v.capacity() == 5);
}

TEST(vector_push_back_test, "vector push back")
{
    tenno::vector<int> v;
    v.push_back(10);
    ASSERT_EQ(v.size(), 1);
    ASSERT_EQ(v.capacity(), 1);
    ASSERT_EQ(v[0].value(), 10);
    v.push_back(20);
    ASSERT_EQ(v.size(), 2);
    ASSERT_EQ(v.capacity(), 2);
    ASSERT_EQ(v[0].value(), 10);
    ASSERT_EQ(v[1].value(), 20);
}

class test_emplace_back
{
  public:
    int val;
    test_emplace_back() : val(0)
    {
    }
    template <class... Args> test_emplace_back(Args &&...args) : val(args...)
    {
    }

    constexpr bool operator==(const test_emplace_back &other) const
    {
        return val == other.val;
    }
    constexpr bool operator!=(const test_emplace_back &other) const
    {
        return val != other.val;
    }
};

TEST(vector_emplace_back_test, "vector emplace back")
{
    tenno::vector<test_emplace_back> v;
    v.emplace_back(10);
    ASSERT_EQ(v.size(), 1);
    ASSERT_EQ(v.capacity(), 1);
    ASSERT_EQ(v[0].value(), 10);
    v.emplace_back(20);
    ASSERT_EQ(v.size(), 2);
    ASSERT_EQ(v.capacity(), 2);
    ASSERT_EQ(v[0].value(), 10);
    ASSERT_EQ(v[1].value(), 20);
}

TEST(vector_pop_back_test, "vector pop back")
{
    tenno::vector<int> v = {1, 2, 3, 4, 5};
    v.pop_back();
    ASSERT_EQ(v.size(), 4);
    ASSERT_EQ(v.capacity(), 5);
    for (int i = 0; i < 4; i++)
    {
        ASSERT_EQ(v[i].value(), i + 1);
    }
}

TEST(vector_resize_test, "vector resize")
{
    tenno::vector<int> v = {1, 2, 3, 4, 5};
    v.resize(3);
    ASSERT_EQ(v.size(), 3);
    ASSERT_EQ(v.capacity(), 5);
    for (int i = 0; i < 3; i++)
    {
        ASSERT_EQ(v[i].value(), i + 1);
    }
    v.resize(5);
    ASSERT_EQ(v.size(), 5);
    ASSERT_EQ(v.capacity(), 5);
    for (int i = 0; i < 3; i++)
    {
        ASSERT_EQ(v[i].value(), i + 1);
    }
    for (int i = 3; i < 5; i++)
    {
        ASSERT_EQ(v[i].value(), 0);
    }
    v.resize(10, 10);
    ASSERT_EQ(v.size(), 10);
    ASSERT_EQ(v.capacity(), 10);
    for (int i = 0; i < 3; i++)
    {
        ASSERT_EQ(v[i].value(), i + 1);
    }
    for (int i = 5; i < 10; i++)
    {
        ASSERT_EQ(v[i].value(), 10);
    }
}

TEST(vector_swap_test, "vector swap")
{
    tenno::vector<int> v1 = {1, 2, 3, 4, 5};
    tenno::vector<int> v2 = {6, 7, 8, 9, 10};
    v1.swap(v2);
    ASSERT_EQ(v1.size(), 5);
    ASSERT_EQ(v1.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v1[i].value(), i + 6);
    }
    ASSERT_EQ(v2.size(), 5);
    ASSERT_EQ(v2.capacity(), 5);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(v2[i].value(), i + 1);
    }
}
