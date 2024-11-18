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

#pragma once

#include <tenno/array.hpp>

namespace tenno
{

inline constexpr unsigned int lcg(const unsigned int seed)
{
  constexpr unsigned int a = 1664525;
  constexpr unsigned int c = 1013904223;
  constexpr unsigned int m =
    std::numeric_limits<unsigned int>::max();
  return (a * seed + c) % m;
}

template<const unsigned int it>
constexpr unsigned int gen(const unsigned int N)
{
  return lcg(gen<it-1>(N));
}

template<>
constexpr unsigned int gen<0>(const unsigned int N)
{
  return lcg(N);
}

template<const unsigned int it>
constexpr float uniform_real_distribution(const int seed,
					  const float min = 0,
					  const float max = 1)
{
  constexpr unsigned int m =
    std::numeric_limits<unsigned int>::max();
  const unsigned int n = gen<it>(seed);
  const float normalized = static_cast<float>(n)
    / static_cast<float>(m);
  return min + normalized * (max - min);
}

template<const tenno::size N, const tenno::size it>
constexpr tenno::array<float, N>
random_vector_impl(const int seed,
		const float min,
		const float max,
		const tenno::array<float, N> vec)
{
  if constexpr (it >= N)
    {
      return vec;
    }
  else
    {
      tenno::array<float, N> new_vec = vec;
      new_vec[it] = uniform_real_distribution<it>(seed, min, max);
      return random_vector_impl<N, it+1>(seed, min, max, new_vec);
    }
}

template<const tenno::size N>
constexpr tenno::array<float, N>
random_vector(const int seed,
	   const float min = 0,
	   const float max = 1)
{
  constexpr tenno::array<float, N> vec;
  return random_vector_impl<N,0>(seed, min, max, vec);
}

} // tenno
