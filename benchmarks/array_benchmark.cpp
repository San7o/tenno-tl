// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <valfuzz/valfuzz.hpp>

// meet the two fighters:
#include <array>
#include <tenno/array.hpp>

BENCHMARK(benchmark_tenno_array_constructor, "tenno::array()")
{
  RUN_BENCHMARK(1000, tenno::array<int, 1000>());
}

BENCHMARK(benchmark_std_array_constructor, "std::array()")
{
  RUN_BENCHMARK(1000, std::array<int, 1000>());
}

BENCHMARK(benchmark_tenno_array_init, "tenno:array{}")
{
  RUN_BENCHMARK(1000, tenno::array<int, 1000>{});
}

BENCHMARK(benchmark_std_array_init, "std::array{}")
{
  RUN_BENCHMARK(1000, std::array<int, 1000>{});
}
