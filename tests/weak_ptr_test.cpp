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

TEST(weak_ptr_empty_constructor, "tenno::weak_ptr empty constructor")
{
    tenno::weak_ptr<int> wp;
    ASSERT(wp.use_count() == 0);
}

TEST(weak_ptr_constructor_from_weak_ptr, "tenno::weak_ptr constructor from weak_ptr")
{
    tenno::shared_ptr<int> sp(new int(42));
    tenno::weak_ptr<int> wp(sp);
    tenno::weak_ptr<int> wp2(wp);
    ASSERT(wp.use_count() == 1);
}

TEST(weak_ptr_constructor_move, "tenno::weak_ptr constructor move")
{
    tenno::shared_ptr<int> sp(new int(42));
    tenno::weak_ptr<int> wp(std::move(sp));
    ASSERT(wp.use_count() == 1);
}

TEST(weak_ptr_constructor_shared_ptr, "tenno::weak_ptr creation")
{
    tenno::shared_ptr<int> sp(new int(42));
    tenno::weak_ptr<int> wp(sp);
    ASSERT(wp.use_count() == 1);
}

TEST(weak_ptr_assignment, "tenno::weak_ptr assignment")
{
    tenno::shared_ptr<int> sp1(new int(42));
    tenno::shared_ptr<int> sp2(new int(42));
    tenno::weak_ptr<int> wp1(sp1);
    tenno::weak_ptr<int> wp2(sp2);
    wp1 = wp2;
    ASSERT(wp1.use_count() == 1);
}

TEST(weal_ptr_assignment_move, "tenno::weak_ptr assignment move")
{
    tenno::shared_ptr<int> sp1(new int(42));
    tenno::shared_ptr<int> sp2(new int(42));
    tenno::weak_ptr<int> wp1(sp1);
    tenno::weak_ptr<int> wp2(sp2);
    wp1 = std::move(wp2);
    ASSERT(wp1.use_count() == 1);
}

TEST(weak_ptr_assignment_shared_ptr, "tenno::weak_ptr assignment from shared_ptr")
{
    tenno::shared_ptr<int> sp(new int(42));
    tenno::weak_ptr<int> wp;
    wp = sp;
    ASSERT(wp.use_count() == 1);
}

TEST(weak_ptr_reset, "tenno::weak_ptr reset")
{
    tenno::shared_ptr<int> sp(new int(42));
    tenno::weak_ptr<int> wp(sp);
    wp.reset();
    ASSERT(wp.use_count() == 0);
}

TEST(weak_ptr_lock, "tenno::weak_ptr lock")
{
    tenno::shared_ptr<int> sp(new int(42));
    tenno::weak_ptr<int> wp(sp);
    tenno::shared_ptr<int> sp2 = wp.lock();
    ASSERT(sp2.use_count() == 2);
}

TEST(weak_ptr_use_count, "tenno::weak_ptr use_count")
{
    tenno::shared_ptr<int> sp(new int(42));
    tenno::weak_ptr<int> wp(sp);
    ASSERT(wp.use_count() == 1);
}

TEST(weak_ptr_expired, "tenno::weak_ptr expired")
{
    tenno::shared_ptr<int> sp(new int(42));
    tenno::weak_ptr<int> wp(sp);
    ASSERT(!wp.expired());
}

TEST(weak_ptr_owner_before, "tenno::weak_ptr owner_before")
{
    tenno::shared_ptr<int> sp1(new int(42));
    tenno::shared_ptr<int> sp2(new int(43));
    tenno::weak_ptr<int> wp1(sp1);
    tenno::weak_ptr<int> wp2(sp2);
    ASSERT(wp1.owner_before(wp2));
}

TEST(weak_ptr_owner_before_shared_ptr, "tenno::weak_ptr owner_before shared_ptr")
{
    tenno::shared_ptr<int> sp1(new int(42));
    tenno::shared_ptr<int> sp2(new int(43));
    tenno::weak_ptr<int> wp1(sp1);
    tenno::weak_ptr<int> wp2(sp2);
    ASSERT(wp1.owner_before(sp2));
}

TEST(weak_ptr_swap, "tenno::weak_ptr swap")
{
    tenno::shared_ptr<int> sp1(new int(42));
    tenno::shared_ptr<int> sp2(new int(43));
    tenno::weak_ptr<int> wp1(sp1);
    tenno::weak_ptr<int> wp2(sp2);
    wp1.swap(wp2);
    ASSERT(wp1.use_count() == 1);
    ASSERT(wp2.use_count() == 1);
}
