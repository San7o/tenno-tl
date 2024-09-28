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
#include <tenno/shared_ptr.hpp>
#include <tenno/utility.hpp>
#include <valfuzz/valfuzz.hpp>

TEST(shared_ptr_constructor, "constructor tenno::shared_ptr")
{
    int a = 10;
    tenno::shared_ptr<int> sp;
    ASSERT_EQ(sp.use_count(), 0);
    {
        auto sp2 = tenno::shared_ptr<int>(&a);
        sp = sp2;
        ASSERT_EQ(sp.use_count(), 2);
    }
    ASSERT_EQ(sp.use_count(), 1);
}

TEST(shared_ptr_assignment, "tenno::shared_ptr = tenno::shared_ptr")
{
    int a = 10;
    auto sp1 = tenno::shared_ptr<int>(&a);
    ASSERT_EQ(sp1.use_count(), 1);
    auto sp2 = sp1;
    ASSERT_EQ(sp1.use_count(), 2);
    auto sp3 = tenno::move(sp2);
    ASSERT_EQ(sp1.use_count(), 2);
}

TEST(shared_ptr_reset0, "tenno::shared_ptr::reset 1")
{
    int a = 10;
    auto sp1 = tenno::shared_ptr<int>(&a);
    auto sp2 = sp1;
    ASSERT_EQ(sp1.use_count(), 2);
    sp1.reset();
    ASSERT_EQ(sp1.use_count(), 0);
    ASSERT_EQ(sp2.use_count(), 1);
}

TEST(shared_ptr_reset1, "tenno::shared_ptr::reset 1")
{
    int a = 10;
    auto sp1 = tenno::shared_ptr<int>(&a);
    ASSERT_EQ(sp1.use_count(), 1);
    int i = 42;
    sp1.reset(&i);
    ASSERT_EQ(sp1.use_count(), 1);
}

TEST(shared_ptr_reset2, "tenno::shared_ptr::reset 2")
{
    int a = 10;
    auto sp = tenno::shared_ptr<int>(&a);
    ASSERT_EQ(*sp.get(), 10);
    int i = 42;
    sp.reset(&i, std::default_delete<tenno::shared_ptr<int>::control_block>());
    ASSERT_EQ(*sp.get(), 42);
}

TEST(shared_ptr_reset3, "tenno::shared_ptr::reset 3")
{
    int a = 10;
    auto sp = tenno::shared_ptr<int>(&a);
    ASSERT_EQ(*sp.get(), 10);
    int i = 42;
    sp.reset(&i, std::default_delete<tenno::shared_ptr<int>::control_block>(), std::allocator<tenno::shared_ptr<int>::control_block>());
    ASSERT_EQ(*sp.get(), 42);
}

TEST(shared_ptr_swap, "tenno::shared_ptr::swap")
{
    int a = 10;
    int b = 11;
    auto sp1 = tenno::shared_ptr<int>(&a);
    auto sp2 = tenno::shared_ptr<int>(&b);
    ASSERT_EQ(sp1.use_count(), 1);
    ASSERT_EQ(sp2.use_count(), 1);
    ASSERT_EQ(*sp1.get(), 10);
    sp1.swap(sp2);
    ASSERT_EQ(sp1.use_count(), 1);
    ASSERT_EQ(sp2.use_count(), 1);
    ASSERT_EQ(*sp1.get(), 11);
}

TEST(shared_ptr_get, "tenno::shared_ptr::get")
{
    int a = 10;
    auto sp = tenno::shared_ptr<int>(&a);
    ASSERT_EQ(*sp.get(), 10);
}

TEST(shared_ptr_dereference_operator, "tenno::shared_ptr::operator*")
{
    int a = 10;
    auto sp = tenno::shared_ptr<int>(&a);
    *sp = 42;
    ASSERT_EQ(*sp.get(), 42);
}

TEST(shared_ptr_arrow_operator, "tenno::shared_ptr::operator->")
{
    struct foo
    {
        int bar;
    } a;
    auto sp = tenno::shared_ptr<foo>(&a);
    sp->bar = 42;
    ASSERT_EQ(sp.get()->bar, 42);
}

TEST(shared_ptr_array_access_operator, "tenno::shared_ptr::operator[]")
{
    auto arr = tenno::array<int, 5>{};
    auto sp = tenno::shared_ptr<tenno::array<int, 5>>(&arr);
    sp[0] = 42;
    ASSERT_EQ((*sp.get())[0], 42);
}

TEST(shared_ptr_use_count, "tenno::shared_ptr::use_count")
{
    int a = 10;
    auto sp1 = tenno::shared_ptr<int>(&a);
    ASSERT_EQ(sp1.use_count(), 1);
}

TEST(shared_ptr_bool, "tenno::shared_ptr::operator bool")
{
    int a = 10;
    auto sp = tenno::shared_ptr<int>(&a);
    ASSERT(sp);
    sp.reset();
    ASSERT(!sp);
}

TEST(shared_ptr_owner_before, "tenno::shared_ptr::owner_before")
{
    int a = 42;
    int b = 43;
    auto sp1 = tenno::shared_ptr<int>(&a);
    auto sp2 = tenno::shared_ptr<int>(&b);
    ASSERT(sp1.owner_before(sp2));
    ASSERT(!sp2.owner_before(sp1));
}

TEST(shared_ptr_owner_equal, "tenno::shared_ptr::owner_equal")
{
    int a = 42;
    int b = 43;
    auto sp1 = tenno::shared_ptr<int>(&a);
    auto sp2 = tenno::shared_ptr<int>(&b);
    auto sp3 = tenno::shared_ptr<int>(&a);
    ASSERT(sp1.owner_equal(sp3));
    ASSERT(!sp1.owner_equal(sp2));
}

TEST(shared_ptr_copy_constructor, "tenno::shared_ptr::shared_ptr copy1")
{
    int a = 10;
    auto sp1 = tenno::shared_ptr<int>(&a);
    auto sp2 = sp1;
    ASSERT_EQ(sp1.use_count(), 2);
}

TEST(shared_ptr_copy_constructor2, "tenno::shared_ptr::shared_ptr copy2")
{
    int a = 10;
    auto sp1 = tenno::shared_ptr<int>(&a);
    ASSERT_EQ(sp1.use_count(), 1);
    auto b = [&test_name](tenno::shared_ptr<int> sp)
    {
        ASSERT(sp.use_count() == 2);
        return;
    };
    b(sp1);
    ASSERT_EQ(sp1.use_count(), 1);
}
