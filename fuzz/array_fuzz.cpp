// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <tenno/array.hpp>
#include <valfuzz/valfuzz.hpp>

FUZZME(nth_fibonacci_fuzz, "Fuzzing tenno::array constructor")
{
  // auto n = (tenno::size) valfuzz::get_random<int>();
  tenno::array<int, 10> arr;
}
