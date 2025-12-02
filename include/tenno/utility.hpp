// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <type_traits> // std::remove_reference_t

namespace tenno
{

/**
 * @brief Move a value from one location to another.
 *
 * @tparam T The type of the value to move.
 * @param t The value to move.
 * @return T&& The moved value.
 */
template <typename T>
constexpr std::remove_reference_t<T> &&move(T &&t) noexcept
{
  return static_cast<typename std::remove_reference<T>::type &&>(t);
}

} // namespace tenno
