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

#include <tenno/unique_ptr.hpp>
#include <tenno/utility.hpp>
#include <valfuzz/valfuzz.hpp>

TEST(unique_ptr_constructor, "tenno::unique_ptr() constructor")
{
    int *p = new int(10);
    auto ptr = tenno::unique_ptr<int>(p);
    ASSERT(ptr.get() != nullptr);
}

TEST(unique_ptr_get, "tenno::unique_ptr.get()")
{
    int *p = new int(10);
    auto ptr = tenno::unique_ptr<int>(p);
    ASSERT(*(ptr.get()) == 10);
}

TEST(unique_ptr_release, "tenno::unique_ptr.release()")
{
    int *p = new int(10);
    auto ptr = tenno::unique_ptr<int>(p);
    ASSERT(*(ptr.get()) == 10);
    int *r = ptr.release();
    ASSERT(ptr.get() == nullptr);
    delete r;
}

TEST(unique_ptr_reset, "tenno::unique_ptr.reset()")
{
    int *p = new int(10);
    auto ptr = tenno::unique_ptr<int>(p);
    ASSERT(*(ptr.get()) == 10);
    ptr.reset();
    ASSERT(ptr.get() == nullptr);
}

TEST(unique_ptr_swap, "tenno::unique_ptr.swap()")
{
    int *p1 = new int(10);
    int *p2 = new int(11);
    auto ptr1 = tenno::unique_ptr<int>(p1);
    auto ptr2 = tenno::unique_ptr<int>(p2);
    ASSERT(*ptr1 == 10);
    ASSERT(*ptr2 == 11);
    ptr1.swap(ptr2);
    ASSERT(*ptr1 == 11);
    ASSERT(*ptr2 == 10);
}

TEST(uniqie_ptr_get_deleter, "tenno::unique_ptr.get_deleter()")
{
    int *p = new int(10);
    auto ptr =
        tenno::unique_ptr<int, void (*)(int *)>(p, [](int *_p) { delete _p; });
    ASSERT(*ptr == 10);
}

TEST(unique_ptr_move_construct, "tenno::unique_ptr move construct")
{
    int *p1 = new int(10);
    auto ptr1 = tenno::unique_ptr<int>(p1);
    ASSERT(*ptr1 == 10);
    auto ptr2 = tenno::move(ptr1);
    ASSERT(ptr1.get() == nullptr);
    ASSERT(ptr2.get() != nullptr);
}

TEST(unique_ptr_move_assignment, "tenno::unique_ptr move assign")
{
    int *p1 = new int(10);
    auto ptr1 = tenno::unique_ptr<int>(p1);
    auto ptr2 = tenno::unique_ptr<int>();
    ptr2 = tenno::move(ptr1);
    ASSERT(ptr1.get() == nullptr);
    ASSERT(ptr2.get() != nullptr);
}
