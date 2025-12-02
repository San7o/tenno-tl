// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <tenno/utility.hpp>

namespace tenno
{

/**
 * @brief Copies the elements in the range [first, last) to the range beginning
 * at d_first.
 *
 * @tparam InputIt The type of the iterator to the first element in the range to
 * copy.
 * @tparam OutputIt The type of the iterator to the first element in the range
 * to copy to.
 * @param first The iterator to the first element in the range to copy.
 * @param last The iterator to the element after the last element in the range
 * to copy.
 * @param d_first The iterator to the first element in the range to copy to.
 * @return OutputIt The iterator to the element after the last element copied.
 */
template <typename InputIt, typename OutputIt>
constexpr OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
{
  while (first != last)
  {
    //        *d_first++ = *first++;
    *d_first = *first;
    d_first++;
    first++;
  }
  return d_first;
}

/**
 * @brief Applies the given function f to the result of dereferencing every
 * element in the range [first, last).
 *
 * @tparam InputIt The type of the iterator to the first element in the range to
 * apply the function to.
 * @tparam UnaryFunc The type of the function to apply to the elements in the
 * range.
 * @param first The iterator to the first element in the range to apply the
 * function to.
 * @param last The iterator to the element after the last element in the range
 * to apply the function to.
 * @param f The function to apply to the elements in the range.
 * @return UnaryFunc The function f.
 */
template <class InputIt, class UnaryFunc>
constexpr UnaryFunc for_each(InputIt first, InputIt last, UnaryFunc f)
{
  while (first != last)
  {
    f(*first++);
  }
  return f;
}

/**
 * @brief Applies the given function f to the result of dereferencing every
 * element in the range [first, last) and returns the result.
 *
 * @tparam InputIt The type of the iterator to the first element in the range to
 * apply the function to.
 * @tparam UnaryFunc The type of the function to apply to the elements in the
 * range.
 * @tparam T The type of the result of the function.
 * @param first The iterator to the first element in the range to apply the
 * function to.
 * @param last The iterator to the element after the last element in the range
 * to apply the function to.
 * @param f The function to apply to the elements in the range.
 * @param init The initial value of the result.
 * @return T The result of the function.
 */
template <class InputIt, class T>
constexpr T accumulate(InputIt first, InputIt last, T init) noexcept
{
  while (first != last)
  {
    init += *first++;
  }
  return init;
}

/**
 * @brief Exchanges the values of a and b.
 *
 * @tparam T The type of the values to swap.
 * @param a The first value to swap.
 * @param b The second value to swap.
 */
template <class T> void swap(T &a, T &b) noexcept
{
  T tmp = tenno::move(a);
  a = tenno::move(b);
  b = tenno::move(tmp);
}

} // namespace tenno
