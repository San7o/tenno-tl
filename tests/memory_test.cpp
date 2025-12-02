// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <tenno/array.hpp>
#include <tenno/memory.hpp>
#include <tenno/vector.hpp>
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
