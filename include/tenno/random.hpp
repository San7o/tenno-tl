// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <limits>
#include <tenno/array.hpp>

namespace tenno
{

inline constexpr unsigned int lcg(const unsigned int seed)
{
  constexpr unsigned int a = 1664525;
  constexpr unsigned int c = 1013904223;
  constexpr unsigned int m = std::numeric_limits<unsigned int>::max();
  return (a * seed + c) % m;
}

template <const unsigned int it>
constexpr unsigned int gen(const unsigned int N)
{
  return lcg(gen<it - 1>(N));
}

template <> constexpr unsigned int gen<0>(const unsigned int N)
{
  return lcg(N);
}

template <const unsigned int it>
constexpr float uniform_real_distribution(const unsigned int seed,
                                          const float min = 0,
                                          const float max = 1)
{
  constexpr unsigned int m = std::numeric_limits<unsigned int>::max();
  const unsigned int n = gen<it>(seed);
  const float normalized = static_cast<float>(n) / static_cast<float>(m);
  return min + normalized * (max - min);
}

template <const tenno::size N, const tenno::size it>
constexpr tenno::array<float, N>
random_array_impl([[maybe_unused]] const int seed,
                  [[maybe_unused]] const float min,
                  [[maybe_unused]] const float max,
                  [[maybe_unused]] const tenno::array<float, N> vec)
{
  if constexpr (it >= N)
  {
    return vec;
  }
  else
  {
    tenno::array<float, N> new_vec = vec;
    new_vec[it] = uniform_real_distribution<it>(seed, min, max);
    return random_array_impl<N, it + 1>(seed, min, max, new_vec);
  }
}

template <const tenno::size N>
constexpr tenno::array<float, N>
random_array(const int seed, const float min = 0, const float max = 1)
{
  constexpr tenno::array<float, N> vec;
  return random_array_impl<N, 0>(seed, min, max, vec);
}

} // namespace tenno
