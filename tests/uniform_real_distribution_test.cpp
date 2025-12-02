// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <tenno/random.hpp>
#include <valfuzz/valfuzz.hpp>

TEST(uniform_real_distribution_constexpr_test,
     "uniform_real_distribution constexpr test")
{
  constexpr unsigned int seed = 1337;
  /*
  static_assert(uniform_real_distribution<1>(seed) == 1.0f);
  static_assert(uniform_real_distribution<2>(seed) == 1.0f);
  static_assert(uniform_real_distribution<3>(seed) == 1.0f);
  static_assert(uniform_real_distribution<4>(seed) == 1.0f);
  */

  // Create array
  const tenno::size N = 100;
  [[maybe_unused]] constexpr tenno::array<float, N> vec =
    tenno::random_array<N>(seed, 0, 1);

  // Print the vector
  /*
  std::cout << "Array: \n";
  for (const auto& c : vec)
    std::cout << c << " ";
  */
}

/*

Notes: I want to write a constexpr uniform random number generator, It
seems like an interesting problem.

First, I need to write some sort of function to gen the next
generation from a previous one.  Remember that anything needs to be
constant so I'll take inspiration from functional programming.

The following code applies a function N times:

```cpp
constexpr int add_one(const int N)
{
  return N+1;
}

template<const int it>
constexpr int gen(const int N)
{
  return add_one(gen<it-1>(N));
}

template<>
constexpr int gen<0>(const int N)
{
  return N;
}

TEST(uniform_real_distribution_constexpr_test,
     "uniform_real_distribution constexpr test")
{
  constexpr int a = 0;
  constexpr int b = gen<3>(a);
  static_assert(b == 3);
}
```

Now I need to change that add_one function to get some sort of random
distribution.  I used lcg like so

```cpp
constexpr int lcg(const int seed)
{
  constexpr unsigned int a = 1664525;
  constexpr unsigned int c = 1013904223;
  constexpr unsigned int m =
    std::numeric_limits<unsigned int>::max();

  return (a * seed + c) % m;
}
```

And at the end I need to to normalize and cast the values:

template<const unsigned int it>
constexpr float uniform_real_distribution(const int seed,
                                          const float min = 0,
                                          const float max = 20)
{
  constexpr unsigned int m =
    std::numeric_limits<unsigned int>::max();
  const unsigned int n = gen<it>(seed);
  const float normalized = static_cast<float>(n)
    / static_cast<float>(m);
  return min + normalized * (max - min);
}

*/
