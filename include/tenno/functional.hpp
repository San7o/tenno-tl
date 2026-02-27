// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <memory> // std::addressof

namespace tenno
{

/**
 * @brief A reference wrapper class
 * @details This class is a wrapper around a reference to an object. It is
 * useful when you need to store a reference in a container, for example.
 *
 * @tparam T
 */
template <class T> class reference_wrapper
{
public:
  using type = T;
  /**
   * @brief Construct a new reference wrapper object
   *
   * @param x
   */
  constexpr reference_wrapper(T &x) noexcept : _ptr(std::addressof(x)) {}

  reference_wrapper(T &&) = delete;
  
  constexpr reference_wrapper() noexcept : _ptr(nullptr)
  {
  }

  constexpr reference_wrapper(const reference_wrapper &other) noexcept
      : _ptr(other._ptr)
  {
  }

  reference_wrapper &operator=(const reference_wrapper &other) noexcept
  {
    _ptr = other._ptr;
    return *this;
  }

  constexpr operator T &() const noexcept
  {
    return *_ptr;
  }

  constexpr T &get() const noexcept
  {
    return *_ptr;
  }

  constexpr bool operator==(const T &&other) const noexcept
  {
    return *_ptr == other;
  }

  constexpr bool operator!=(const T &&other) const noexcept
  {
    return *_ptr != other;
  }

  constexpr T* operator->() noexcept
  {
    return _ptr;
  }

private:
  T *_ptr;
};


  
} // namespace tenno
