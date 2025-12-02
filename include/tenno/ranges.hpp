// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <cstddef>  // for std::ptrdiff_t
#include <iterator> // for std::forward_iterator_tag
#include <tenno/types.hpp>

namespace tenno
{

/**
 * @brief A range of elements
 *
 * @tparam T The type of the elements in the range
 */
template <typename T> class range
{
public:
  T start_elem;
  T end_elem;

  constexpr range(T start, T end) : start_elem(start), end_elem(end)
  {
  }

  constexpr explicit range(T end) : start_elem(0), end_elem(end)
  {
  }

  constexpr tenno::size size() const noexcept
  {
    return (tenno::size) end_elem - start_elem;
  }

  struct iterator
  {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;

    T current;
    constexpr explicit iterator(T current_it) : current(current_it)
    {
    }

    constexpr T operator*() const noexcept
    {
      return current;
    }

    constexpr iterator &operator++() noexcept
    {
      ++current;
      return *this;
    }

    constexpr iterator operator++(int) noexcept
    {
      iterator _iterator = *this;
      ++*this;
      return _iterator;
    }

    constexpr bool operator!=(const iterator &other) const noexcept
    {
      return current != other.current;
    }
  };

  constexpr iterator begin() const noexcept
  {
    return iterator(start_elem);
  }

  constexpr iterator end() const noexcept
  {
    return iterator(end_elem);
  }

}; // class range

} // namespace tenno
