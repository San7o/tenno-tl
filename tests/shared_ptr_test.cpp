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

#include <tenno/shared_ptr.hpp>
#include <tenno/utility.hpp>
#include <tenno/array.hpp>
#include <valfuzz/valfuzz.hpp>

TEST(shared_ptr_constructor, "tenno::shared_ptr(cb)")
{
    auto cb = tenno::shared_ptr<int>::control_block();
    {
        auto sp = tenno::shared_ptr<int>(&cb);
        ASSERT_EQ(cb.num_ptrs, 1);
    }
    ASSERT_EQ(cb.num_ptrs, 0);
}

TEST(shared_ptr_assignment, "tenno::shared_ptr = tenno::shared_ptr")
{
    auto cb = tenno::shared_ptr<int>::control_block();
    auto sp1 = tenno::shared_ptr<int>(&cb);
    auto sp2 = tenno::shared_ptr<int>(&cb);
    ASSERT_EQ(cb.num_ptrs, 2);
    auto sp3 = tenno::move(sp2);
    ASSERT_EQ(cb.num_ptrs, 2);
}

TEST(shared_ptr_reset0, "tenno::shared_ptr::reset 1")
{
    auto cb = tenno::shared_ptr<int>::control_block();
    auto sp1 = tenno::shared_ptr<int>(&cb);
    auto sp2 = tenno::shared_ptr<int>(&cb);
    ASSERT_EQ(cb.num_ptrs, 2);
    sp1.reset();
    ASSERT_EQ(cb.num_ptrs, 1);
}

TEST(shared_ptr_reset1, "tenno::shared_ptr::reset 1")
{
    auto cb = tenno::shared_ptr<int>::control_block();
    auto sp1 = tenno::shared_ptr<int>(&cb);
    ASSERT_EQ(cb.num_ptrs, 1);
    sp1.reset(&cb);
    ASSERT_EQ(cb.num_ptrs, 1);
}

TEST(shared_ptr_reset2, "tenno::shared_ptr::reset 2")
{
    auto cb = tenno::shared_ptr<int>::control_block();
    auto sp = tenno::shared_ptr<int>(&cb);
    ASSERT_EQ(cb.num_ptrs, 1);
    sp.reset(&cb, nullptr);
    ASSERT_EQ(cb.num_ptrs, 1);
}

TEST(shared_ptr_reset3, "tenno::shared_ptr::reset 3")
{
    auto cb1 = tenno::shared_ptr<int>::control_block();
    auto cb2 = tenno::shared_ptr<int>::control_block();
    auto sp = tenno::shared_ptr<int>(&cb1);
    ASSERT_EQ(cb1.num_ptrs, 1);
    ASSERT_EQ(cb2.num_ptrs, 0);
    sp.reset(&cb2, nullptr, nullptr);
    ASSERT_EQ(cb1.num_ptrs, 0);
    ASSERT_EQ(cb2.num_ptrs, 1);
}

TEST(shared_ptr_swap, "tenno::shared_ptr::swap")
{
    auto cb1 = tenno::shared_ptr<int>::control_block();
    auto cb2 = tenno::shared_ptr<int>::control_block();
    auto sp1 = tenno::shared_ptr<int>(&cb1);
    auto sp2 = tenno::shared_ptr<int>(&cb2);
    ASSERT_EQ(cb1.num_ptrs, 1);
    ASSERT_EQ(cb2.num_ptrs, 1);
    sp1.swap(sp2);
    ASSERT_EQ(cb1.num_ptrs, 1);
    ASSERT_EQ(cb2.num_ptrs, 1);
}

TEST(shared_ptr_get, "tenno::shared_ptr::get")
{
    auto cb = tenno::shared_ptr<int>::control_block();
    auto sp = tenno::shared_ptr<int>(&cb);
    ASSERT_EQ(sp.get(), &cb.object);
}

TEST(shared_ptr_dereference_operator, "tenno::shared_ptr::operator*")
{
    auto cb = tenno::shared_ptr<int>::control_block();
    auto sp = tenno::shared_ptr<int>(&cb);
    *sp = 42;
    ASSERT_EQ(cb.object, 42);
}

TEST(shared_ptr_arrow_operator, "tenno::shared_ptr::operator->")
{
    struct foo
    {
        int bar;
    };
    auto cb = tenno::shared_ptr<foo>::control_block();
    auto sp = tenno::shared_ptr<foo>(&cb);
    sp->bar = 42;
    ASSERT_EQ(cb.object.bar, 42);
}

TEST(shared_ptr_array_access_operator, "tenno::shared_ptr::operator[]")
{
    auto cb = tenno::shared_ptr<tenno::array<int, 5>>::control_block();
    cb.object[0] = 0;
    auto sp = tenno::shared_ptr<tenno::array<int, 5>>(&cb);
    sp[0] = 42;
    ASSERT_EQ(cb.object[0], 42);
}

TEST(shared_ptr_use_count, "tenno::shared_ptr::use_count")
{
    auto cb = tenno::shared_ptr<int>::control_block();
    auto sp1 = tenno::shared_ptr<int>(&cb);
    auto sp2 = tenno::shared_ptr<int>(&cb);
    ASSERT_EQ(sp1.use_count(), 2);
    ASSERT_EQ(sp2.use_count(), 2);
}

TEST(shared_ptr_unique, "tenno::shared_ptr::unique")
{
    auto cb = tenno::shared_ptr<int>::control_block();
    auto sp1 = tenno::shared_ptr<int>(&cb);
    auto sp2 = tenno::shared_ptr<int>(&cb);
    ASSERT(!sp1.unique());
    ASSERT(!sp2.unique());
    sp2.reset();
    ASSERT(sp1.unique());
}


TEST(shared_ptr_bool, "tenno::shared_ptr::operator bool")
{
    auto cb = tenno::shared_ptr<int>::control_block();
    auto sp = tenno::shared_ptr<int>(&cb);
    ASSERT(sp);
    sp.reset();
    ASSERT(!sp);
}

TEST(shared_ptr_owner_before, "tenno::shared_ptr::owner_before")
{
    auto cb1 = tenno::shared_ptr<int>::control_block();
    cb1.object = 42;
    auto cb2 = tenno::shared_ptr<int>::control_block();
    cb2.object = 43;
    auto sp1 = tenno::shared_ptr<int>(&cb1);
    auto sp2 = tenno::shared_ptr<int>(&cb2);
    ASSERT(sp1.owner_before(sp2));
    ASSERT(!sp2.owner_before(sp1));
}

TEST(shared_ptr_owner_equal, "tenno::shared_ptr::owner_equal")
{
    auto cb1 = tenno::shared_ptr<int>::control_block();
    cb1.object = 42;
    auto cb2 = tenno::shared_ptr<int>::control_block();
    cb2.object = 43;
    auto sp1 = tenno::shared_ptr<int>(&cb1);
    auto sp2 = tenno::shared_ptr<int>(&cb2);
    auto sp3 = tenno::shared_ptr<int>(&cb1);
    ASSERT(sp1.owner_equal(sp3));
    ASSERT(!sp1.owner_equal(sp2));
}

TEST(shared_ptr_copy_constructor, "tenno::shared_ptr::shared_ptr copy1")
{
    auto cb = tenno::shared_ptr<int>::control_block();
    auto sp1 = tenno::shared_ptr<int>(&cb);
    auto sp2 = tenno::shared_ptr<int>(sp1);
    ASSERT_EQ(cb.num_ptrs, 2);
}

TEST(shared_ptr_copy_constructor2, "tenno::shared_ptr::shared_ptr copy2")
{
    auto cb = tenno::shared_ptr<int>::control_block();
    auto sp1 = tenno::shared_ptr<int>(&cb);
    ASSERT_EQ(cb.num_ptrs, 1);
    auto a = [&test_name](tenno::shared_ptr<int> sp) {
        ASSERT(sp.use_count() == 2);
        return;
    };
    a(sp1);
    ASSERT_EQ(cb.num_ptrs, 1);
}
