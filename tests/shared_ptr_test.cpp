// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <tenno/array.hpp>
#include <tenno/memory.hpp>
#include <tenno/utility.hpp>
#include <valfuzz/valfuzz.hpp>

TEST(shared_ptr_constructor, "constructor tenno::shared_ptr")
{
  tenno::shared_ptr<int> sp;
  ASSERT_EQ(sp.use_count(), 0);
  {
    auto sp2 = tenno::shared_ptr<int>(new int(10));
    sp = sp2;
    ASSERT_EQ(sp.use_count(), 2);
  }
  ASSERT_EQ(sp.use_count(), 1);
}

TEST(shared_ptr_assignment, "tenno::shared_ptr = tenno::shared_ptr")
{
  auto sp1 = tenno::shared_ptr<int>(new int(10));
  ASSERT_EQ(sp1.use_count(), 1);
  auto sp2 = sp1;
  ASSERT_EQ(sp1.use_count(), 2);
  auto sp3 = tenno::move(sp2);
  ASSERT_EQ(sp1.use_count(), 2);
}

TEST(shared_ptr_reset, "tenno::shared_ptr::reset")
{
  auto sp1 = tenno::shared_ptr<int>(new int(10));
  auto sp2 = sp1;
  ASSERT_EQ(sp1.use_count(), 2);
  sp1.reset();
  ASSERT_EQ(sp1.use_count(), 0);
  ASSERT_EQ(sp2.use_count(), 1);
}

TEST(shared_ptr_swap, "tenno::shared_ptr::swap")
{
  auto sp1 = tenno::shared_ptr<int>(new int(10));
  auto sp2 = tenno::shared_ptr<int>(new int(11));
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
  auto sp = tenno::shared_ptr<int>(new int(10));
  ASSERT_EQ(*sp.get(), 10);
}

TEST(shared_ptr_dereference_operator, "tenno::shared_ptr::operator*")
{
  auto sp = tenno::shared_ptr<int>(new int(10));
  *sp = 42;
  ASSERT_EQ(*sp.get(), 42);
}

TEST(shared_ptr_arrow_operator, "tenno::shared_ptr::operator->")
{
  struct foo
  {
    int bar;
  };
  auto sp = tenno::shared_ptr<foo>(new foo());
  sp->bar = 42;
  ASSERT_EQ(sp.get()->bar, 42);
}

TEST(shared_ptr_array_access_operator, "tenno::shared_ptr::operator[]")
{
  auto sp = tenno::shared_ptr<tenno::array<int, 5>>(new tenno::array<int, 5>{});
  for (int i = 0; i < 5; i++)
  {
    sp[i] = i;
  }
  ASSERT_EQ((*sp.get())[0], 0);
  ASSERT_EQ((*sp.get())[1], 1);
  ASSERT_EQ((*sp.get())[2], 2);
}

TEST(shared_ptr_use_count, "tenno::shared_ptr::use_count")
{
  auto sp1 = tenno::shared_ptr<int>(new int(10));
  ASSERT_EQ(sp1.use_count(), 1);
}

TEST(shared_ptr_bool, "tenno::shared_ptr::operator bool")
{
  auto sp = tenno::shared_ptr<int>(new int(10));
  ASSERT(sp);
  sp.reset();
  ASSERT(!sp);
}

TEST(shared_ptr_owner_before, "tenno::shared_ptr::owner_before")
{
  auto sp1 = tenno::shared_ptr<int>(new int(42));
  auto sp2 = tenno::shared_ptr<int>(new int(43));
  ASSERT(sp2.owner_before(sp1));
  ASSERT(!sp1.owner_before(sp2));
}

TEST(shared_ptr_owner_equal, "tenno::shared_ptr::owner_equal")
{
  auto sp1 = tenno::shared_ptr<int>(new int(42));
  auto sp2 = tenno::shared_ptr<int>(new int(43));
  auto sp3 = tenno::shared_ptr<int>(new int(42));
  ASSERT(sp1.owner_equal(sp1));
  ASSERT(!sp1.owner_equal(sp2));
}

TEST(shared_ptr_copy_constructor, "tenno::shared_ptr::shared_ptr copy1")
{
  auto sp1 = tenno::shared_ptr<int>(new int(10));
  auto sp2 = sp1;
  ASSERT_EQ(sp1.use_count(), 2);
}

TEST(shared_ptr_copy_constructor2, "tenno::shared_ptr::shared_ptr copy2")
{
  auto sp1 = tenno::shared_ptr<int>(new int(10));
  ASSERT_EQ(sp1.use_count(), 1);
  auto b = [&test_name](tenno::shared_ptr<int> sp)
  {
    ASSERT(sp.use_count() == 2);
    return;
  };
  b(sp1);
  ASSERT_EQ(sp1.use_count(), 1);
}
