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

#include <tenno/array.hpp>
#include <tenno/types.hpp>
#include <valfuzz/valfuzz.hpp>

TEST(array_create, "creating tenno::array")
{
    [[maybe_unused]] auto arr = tenno::array<int, 5>();
}

TEST(array_value_type, "reading tenno::array::value_type")
{
    ASSERT_EQ(typeid(tenno::array<int, 5>::value_type), typeid(int));
}

TEST(array_size, "reading tenno::array.size()")
{
    auto arr = tenno::array<int, 5>();
    ASSERT_EQ(arr.size(), 5);
}

TEST(array_default_init, "initialize tenno::array with default values")
{
    auto arr = tenno::array<int, 5>::init();
    ASSERT_EQ(arr.data()[0], 0);
    ASSERT_EQ(arr.data()[1], 0);
    ASSERT_EQ(arr.data()[2], 0);
    ASSERT_EQ(arr.data()[3], 0);
    ASSERT_EQ(arr.data()[4], 0);
}

TEST(array_initialize_lize, "initialize tenno::array with a list")
{
    tenno::array<int, 5> arr{1, 2, 3, 4, 5};
    ASSERT_EQ(arr.data()[0], 1);
    ASSERT_EQ(arr.data()[1], 2);
    ASSERT_EQ(arr.data()[2], 3);
    ASSERT_EQ(arr.data()[3], 4);
    ASSERT_EQ(arr.data()[4], 5);
    tenno::array<int, 5> arr2 = {5, 4, 3, 2, 1};
    ASSERT_EQ(arr2.data()[0], 5);
    ASSERT_EQ(arr2.data()[1], 4);
    ASSERT_EQ(arr2.data()[2], 3);
    ASSERT_EQ(arr2.data()[3], 2);
    ASSERT_EQ(arr2.data()[4], 1);
}

TEST(array_iterator_begin, "tenno::array.begin()")
{
    auto arr = tenno::array<int, 5>{1, 2, 3, 4, 5};
    auto it = arr.begin();
    ASSERT_EQ(*it, 1);
}

TEST(array_iterator_dereference, "tenno::array::iterator::operator*()")
{
    auto arr = tenno::array<int, 3>{1, 2, 3};
    auto it = arr.begin();
    ASSERT_EQ(*it, 1);
}

TEST(array_iterator_end, "tenno::array.end()")
{
    auto arr = tenno::array<int, 3>{1, 2, 3};
    auto it = arr.end();
    ASSERT_NE(*it, 1);
    ASSERT_NE(*it, 2);
    ASSERT_NE(*it, 3);
}

TEST(array_iterator_increment, "tenno::array::iterator::operator++()")
{
    auto arr = tenno::array<int, 3>{1, 2, 3};
    auto it = arr.begin();
    ++it;
    ASSERT_EQ(*it, 2);
    ++it;
    ASSERT_EQ(*it, 3);
}

TEST(array_interator_pre_increment, "tenno::array::iterator::operator++(int)")
{
    auto arr = tenno::array<int, 3>{1, 2, 3};
    auto it = arr.begin();
    auto it2 = it++;
    ASSERT_EQ(*it2, 1);
    ASSERT_EQ(*it, 2);
}

TEST(array_iterator_equality, "tenno::array::iterator::operator==()")
{
    auto arr = tenno::array<int, 3>{1, 2, 3};
    auto it = arr.begin();
    auto it2 = arr.begin();
    ASSERT_EQ(it, it2);
    ++it;
    ASSERT_NE(it, it2);
}

TEST(array_iterator_inequality, "tenno::array::iterator::operator!=()")
{
    auto arr = tenno::array<int, 3>{1, 2, 3};
    auto it = arr.begin();
    auto it2 = arr.begin();
    ASSERT_EQ(it, it2);
    ++it;
    ASSERT_NE(it, it2);
}

TEST(array_iteration, "iterating over tenno::array")
{
    auto arr = tenno::array<int, 3>{1, 2, 3};
    auto sum = 0;
    for (auto elem : arr)
    {
        sum += elem;
    }
    ASSERT_EQ(sum, 6);
}

TEST(array_reverse_iteration, "reverse iterating over tenno::array")
{
    auto arr = tenno::array<int, 3>{1, 2, 3};
    auto sum = 0;
    for (auto it = arr.rbegin(); it != arr.rend();)
    {
        sum += *it;
        ++it;
    }
    ASSERT_EQ(sum, 6);
    auto rit = arr.rbegin();
    ASSERT_EQ(*rit, 3);
    rit++;
    ASSERT_EQ(*rit, 2);
    rit++;
    ASSERT_EQ(*rit, 1);
}

TEST(array_constexpr, "create tenno::array constexpr")
{
    constexpr auto arr = tenno::array<int, 3>{1, 2, 3};
    static_assert(arr.data()[0] == 1);
    static_assert(arr.data()[1] == 2);
    static_assert(arr.data()[2] == 3);
}

TEST(array_constexpr_init,
     "initialize tenno::array with default values constexpr")
{
    constexpr auto arr = tenno::array<int, 3>::init();
    static_assert(arr.data()[0] == 0);
    static_assert(arr.data()[1] == 0);
    static_assert(arr.data()[2] == 0);
}

TEST(array_constexpr_iterator, "iterate over tenno::array constexpr")
{
    constexpr auto arr = tenno::array<int, 3>{1, 2, 3};
    constexpr auto sum = [&arr]()
    {
        auto sum_ = 0;
        for (auto elem : arr)
        {
            sum_ += elem;
        }
        return sum_;
    }();
    static_assert(sum == 6);
}

TEST(array_at, "tenno::array::at()")
{
    auto arr = tenno::array<int, 3>{1, 2, 3};
    ASSERT(arr.at(0).has_value());
    ASSERT_EQ(arr.at(0).value(), 1);
    ASSERT(arr.at(1).has_value());
    ASSERT_EQ(arr.at(1).value(), 2);
    ASSERT(arr.at(2).has_value());
    ASSERT_EQ(arr.at(2).value(), 3);
}

TEST(array_at_error, "tenno::array::at() error")
{
    auto arr = tenno::array<int, 3>{1, 2, 3};
    ASSERT(!arr.at(3).has_value());
    ASSERT_EQ(arr.at(3).error(), tenno::error::out_of_range);
}

TEST(array_at_constexpr, "tenno::array::at() constexpr")
{
    constexpr auto arr = tenno::array<int, 3>{1, 2, 3};
    static_assert(arr.at(0).has_value());
    static_assert(arr.at(0).value() == 1);
    static_assert(arr.at(1).has_value());
    static_assert(arr.at(1).value() == 2);
    static_assert(arr.at(2).has_value());
    static_assert(arr.at(2).value() == 3);
}

TEST(array_at_error_constexpr, "tenno::array::at() error constexpr")
{
    constexpr auto arr = tenno::array<int, 3>{1, 2, 3};
    static_assert(!arr.at(3).has_value());
    static_assert(arr.at(3).error() == tenno::error::out_of_range);
}

TEST(array_at_mutable, "tenno::array::at() mutable")
{
    auto arr = tenno::array<int, 3>{1, 2, 3};
    auto val = arr.at(0).value();
    val++;
    ASSERT_EQ(arr.at(0).value(), 1);
}

TEST(array_front, "tenno::array::front()")
{
    auto arr = tenno::array<int, 3>{1, 2, 3};
    ASSERT_EQ(arr.front(), 1);
}

TEST(array_front_constexpr, "tenno::array::front() constexpr")
{
    constexpr auto arr = tenno::array<int, 3>{1, 2, 3};
    static_assert(arr.front() == 1);
}

TEST(array_back, "tenno::array::back()")
{
    auto arr = tenno::array<int, 3>{1, 2, 3};
    ASSERT_EQ(arr.back(), 3);
}

TEST(array_back_constexpr, "tenno::array::back() constexpr")
{
    constexpr auto arr = tenno::array<int, 3>{1, 2, 3};
    static_assert(arr.back() == 3);
}

TEST(array_data, "tenno::array::data()")
{
    auto arr = tenno::array<int, 3>{1, 2, 3};
    auto data = arr.data();
    ASSERT_EQ(data[0], 1);
    ASSERT_EQ(data[1], 2);
    ASSERT_EQ(data[2], 3);
}

TEST(array_empty, "tenno::array::empty()")
{
    auto arr = tenno::array<int, 3>{1, 2, 3};
    ASSERT(!arr.empty());
}

TEST(array_empty_constexpr, "tenno::array::empty() constexpr")
{
    constexpr auto arr = tenno::array<int, 3>{1, 2, 3};
    static_assert(!arr.empty());
}

TEST(array_max_size, "tenno::array::max_size()")
{
    auto arr = tenno::array<int, 3>{1, 2, 3};
    ASSERT_EQ(arr.max_size(), 3);
}

TEST(arrya_max_size_constexpr, "tenno::array::max_size() constexpr")
{
    constexpr auto arr = tenno::array<int, 3>{1, 2, 3};
    static_assert(arr.max_size() == 3);
}

TEST(array_fill, "tenno::array::fill()")
{
    auto arr = tenno::array<int, 3>{1, 2, 3};
    arr.fill(0);
    ASSERT_EQ(arr.data()[0], 0);
    ASSERT_EQ(arr.data()[1], 0);
    ASSERT_EQ(arr.data()[2], 0);
}

TEST(array_swap, "tenno::array::swap()")
{
    auto arr = tenno::array<int, 3>{1, 2, 3};
    auto arr2 = tenno::array<int, 3>{4, 5, 6};
    arr.swap(arr2);
    ASSERT_EQ(arr.data()[0], 4);
    ASSERT_EQ(arr.data()[1], 5);
    ASSERT_EQ(arr.data()[2], 6);
    ASSERT_EQ(arr2.data()[0], 1);
    ASSERT_EQ(arr2.data()[1], 2);
    ASSERT_EQ(arr2.data()[2], 3);
}

TEST(array_operator_brackets, "tenno::array::operator[]")
{
    auto arr = tenno::array<int, 3>{1, 2, 3};
    ASSERT_EQ(arr[0], 1);
    ASSERT_EQ(arr[1], 2);
    ASSERT_EQ(arr[2], 3);
}

TEST(array_operator_brackets_mutable, "tenno::array::operator[] mutable")
{
    auto arr = tenno::array<int, 3>{1, 2, 3};
    arr[0] = 0;
    arr[1] = 0;
    arr[2] = 0;
    ASSERT_EQ(arr[0], 0);
    ASSERT_EQ(arr[1], 0);
    ASSERT_EQ(arr[2], 0);
}

TEST(array_operator_brackets_constexpr, "tenno::array::operator[] constexpr")
{
    constexpr auto arr = tenno::array<int, 3>{1, 2, 3};
    static_assert(arr[0] == 1);
    static_assert(arr[1] == 2);
    static_assert(arr[2] == 3);
}
