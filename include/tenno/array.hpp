// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <cstddef>          /* for std::ptrdiff_t */
#include <initializer_list> /* brace-enclosed initializer list must use std */
#include <iterator>         /* for std::forward_iterator_tag */
#include <tenno/algorithm.hpp>
#include <tenno/error.hpp>
#include <tenno/expected.hpp>
#include <tenno/ranges.hpp>
#include <tenno/types.hpp>

namespace tenno
{

/**
 * @brief A fixed-size array
 *
 * @tparam T The type of the elements in the array
 * @tparam N The size of the array
 *
 * # Example
 * ```cpp
 * auto arr = tenno::array<int, 5>();
 * ```
 * No memory allocation is performed, and the array is stored on the stack.
 */
template <typename T, tenno::size N> class array
{
public:
  /**
   * @brief The type of the elements in the array
   */
  using value_type = T;

  /* TODO: may be removed */
  static constexpr T generate_default()
  {
    return T{}; // Default-initialize T
  }

  /**
   * @brief Construct a new array object
   *
   * By default, the array's elements are not initialized for
   * performance reasons. If you want to initialize the array's
   * elements, use the static `init` method.
   */
  constexpr array() : _data{}
  {
    for (auto i : tenno::range<tenno::size>(N))
    {
      this->_data[i] = generate_default();
    }
  }

  /**
   * @brief Construct a new array object with a list of elements
   *
   * @param list The list of elements to initialize the array with
   *
   * # Example
   * ```cpp
   * tenno::array<int, 5> arr{1, 2, 3, 4, 5};
   * tenno::array<int, 5> arr2 = {5, 4, 3, 2, 1};
   * ```
   */
  constexpr array(std::initializer_list<T> list)
  {
    tenno::copy(list.begin(), list.end(), this->_data);
  }

  constexpr array(const array &other) : _data{}
  {
    tenno::copy(other.cbegin(), other.cend(), this->_data);
  }

  /**
   * @brief Get the size of the array
   *
   * @return tenno::size The size of the array
   *
   * # Example
   * ```cpp
   * auto arr = tenno::array<int, 5>();
   * const tenno::size size = arr.size();
   * ```
   */
  constexpr tenno::size size() const noexcept
  {
    return this->_size;
  }

  /**
   * @brief Initialize all elements of the array to the default value of T
   *
   * # Example
   * ```cpp
   * auto arr = tenno::array<int, 10>::init();
   * ```
   */
  static constexpr tenno::array<T, N> init() noexcept
  {
    auto arr = tenno::array<T, N>();
    for (auto i : tenno::range<tenno::size>(N))
    {
      arr._data[i] = T();
    }
    return arr;
  }

  /**
   * @brief Access an element of the array
   *
   * @param index The index of the element to access
   * @return tenno::expected<T, tenno::error> The element at the index
   *
   * # Example
   * ```cpp
   * auto arr = tenno::array<int, 5>{1, 2, 3, 4, 5};
   * auto elem = arr.at(2);
   * if (elem.has_value())
   * {
   *     std::cout << elem.value() << std::endl;
   * }
   * ```
   */
  constexpr tenno::expected<T, tenno::error>
  at(const tenno::size index) const noexcept
  {
    if (index >= this->_size)
    {
      return tenno::unexpected(tenno::error::out_of_range);
    }
    return this->_data[index];
  }

  /**
   * @brief Access an element of the array
   *
   * @param index The index of the element to access
   * @return T The element at the index
   *
   * # Example
   * ```cpp
   * auto arr = tenno::array<int, 5>{1, 2, 3, 4, 5};
   * auto elem = arr[2];
   * ```
   */
  constexpr T front() const noexcept
  {
    return this->_data[0];
  }

  /**
   * @brief Access an element of the array
   *
   * @param index The index of the element to access
   * @return T The element at the index
   *
   * # Example
   * ```cpp
   * auto arr = tenno::array<int, 5>{1, 2, 3, 4, 5};
   * auto elem = arr[2];
   * ```
   */
  constexpr T back() const noexcept
  {
    return this->_data[this->_size - 1];
  }

  /**
   * @brief Access an element of the array
   *
   * @param index The index of the element to access
   * @return T The element at the index
   *
   * # Example
   * ```cpp
   * auto arr = tenno::array<int, 5>{1, 2, 3, 4, 5};
   * auto elem = arr[2];
   * ```
   */
  T *data() noexcept
  {
    return this->_data;
  }

  /**
   * @brief Access the underlying data of the array
   * @return const T* The underlying data of the array
   */
  constexpr const T *data() const noexcept
  {
    return this->_data;
  }

  /**
   * @brief Check if the array is empty
   *
   * @return true If the array is empty
   * @return false If the array is not empty
   *
   * # Example
   * ```cpp
   * auto arr = tenno::array<int, 5>();
   * if (arr.empty())
   * {
   *     std::cout << "The array is empty" << std::endl;
   * }
   * ```
   */
  constexpr bool empty() const noexcept
  {
    return this->_size == 0;
  }

  /**
   * @brief Get the maximum size of the array
   *
   * @return tenno::size The maximum size of the array
   *
   * # Example
   * ```cpp
   * auto arr = tenno::array<int, 5>();
   * const tenno::size max_size = arr.max_size();
   * ```
   */
  constexpr tenno::size max_size() const noexcept
  {
    return this->_size;
  }

  /**
   * @brief Fill the array with a value
   *
   * @param value The value to fill the array with
   *
   * # Example
   * ```cpp
   * auto arr = tenno::array<int, 5>();
   * arr.fill(0);
   * ```
   */
  void fill(const T &value) noexcept
  {
    tenno::for_each(this->begin(), this->end(),
                    [&value](T &elem) { elem = value; });
  }

  /**
   * @brief Swap the contents of the array with another array
   *
   * @param other The other array to swap with
   *
   * # Example
   * ```cpp
   * auto arr = tenno::array<int, 3>{1, 2, 3};
   * auto arr2 = tenno::array<int, 3>{4, 5, 6};
   * arr.swap(arr2);
   * ```
   */
  void swap(array &other) noexcept
  {
    for (auto i : tenno::range<tenno::size>(this->_size))
    {
      auto temp = this->_data[i];
      this->_data[i] = other._data[i];
      other._data[i] = temp;
    }
  }

  /**
   * @brief Access an element of the array
   *
   * @param index The index of the element to access
   * @return T The element at the index
   *
   * # Example
   * ```cpp
   * auto arr = tenno::array<int, 5>{1, 2, 3, 4, 5};
   * auto elem = arr[2];
   * ```
   */
  constexpr T &operator[](const tenno::size index) noexcept
  {
    return this->_data[index];
  }

  /**
   * @brief Access an element of the array constexpr
   */
  constexpr const T &operator[](const tenno::size index) const noexcept
  {
    return this->_data[index];
  }

  constexpr array &operator=(const array &other) noexcept
  {
    tenno::copy(other.begin(), other.end(), this->_data);
    return *this;
  }

  /**
   * @brief An iterator to iterate over the array
   */
  struct iterator
  {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;

    tenno::array<T, N> &array;
    tenno::size index;

    explicit iterator(tenno::array<T, N> &array_in, tenno::size end) noexcept
        : array(array_in), index(end)
    {
    }

    iterator &operator++() noexcept
    {
      ++index;
      return *this;
    }

    iterator operator++(int) noexcept
    {
      iterator _iterator = *this;
      ++index;
      return _iterator;
    }

    bool operator==(const iterator &other) const noexcept
    {
      return index == other.index;
    }

    bool operator!=(const iterator &other) const noexcept
    {
      return !(*this == other);
    }

    reference operator*() const noexcept
    {
      return array[index];
    }
  };

  /**
   * @brief Get an iterator to the beginning of the array
   *
   * @return iterator The iterator to the beginning of the array
   *
   * # Example
   * ```cpp
   * auto arr = tenno::array<int, 5>();
   * auto begin = arr.begin();
   * ```
   */
  iterator begin() noexcept
  {
    return iterator(*this, 0);
  }
  /**
   * @brief Get an iterator to the end of the array
   *
   * @return iterator The iterator to the end of the array
   *
   * # Example
   * ```cpp
   * tenno::for_each(arr.begin(), arr.end(), [](int& elem) { elem = 0; });
   * ```
   */
  iterator end() noexcept
  {
    return iterator(*this, this->_size);
  }

  /**
   * @brief A const iterator to iterate over the array
   */
  struct const_iterator
  {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;

    const tenno::array<T, N> &array;
    tenno::size index;

    constexpr explicit const_iterator(const tenno::array<T, N> &array_in,
                                      const tenno::size end) noexcept
        : array(array_in), index(end)
    {
    }

    constexpr const_iterator &operator++() noexcept
    {
      index += 1;
      return *this;
    }

    constexpr const_iterator operator++(int) noexcept
    {
      const_iterator _const_iterator = *this;
      index += 1;
      return _const_iterator;
    }

    constexpr bool operator==(const const_iterator &other) const noexcept
    {
      return index == other.index;
    }

    constexpr bool operator!=(const const_iterator &other) const noexcept
    {
      return !(*this == other);
    }

    constexpr T operator*() const noexcept
    {
      return array[index];
    }
  };

  /**
   * @brief Get an const_iterator to the beginning of the array
   *
   * @return const_iterator The iterator to the beginning of the array
   *
   * # Example
   * ```cpp
   * auto arr = tenno::array<int, 5>();
   * auto begin = arr.begin();
   * ```
   */
  constexpr const_iterator cbegin() const noexcept
  {
    return const_iterator(*this, 0);
  }
  /**
   * @brief Get an const_iterator to the end of the array
   *
   * @return const_iterator The iterator to the end of the array
   *
   * # Example
   * ```cpp
   * tenno::for_each(arr.begin(), arr.end(), [](int& elem) { elem = 0; });
   * ```
   */
  constexpr const_iterator cend() const noexcept
  {
    return const_iterator(*this, this->_size);
  }

  /**
   * @brief A reverse iterator over the array
   */
  struct reverse_iterator
  {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;

    tenno::array<T, N> &array;
    tenno::size index;

    explicit reverse_iterator(tenno::array<T, N> &array_in,
                              tenno::size end) noexcept
        : array(array_in), index(end)
    {
    }

    reverse_iterator &operator++() noexcept
    {
      --index;
      return *this;
    }

    reverse_iterator operator++(int) noexcept
    {
      reverse_iterator _reverse_iterator = *this;
      --index;
      return _reverse_iterator;
    }

    bool operator==(const reverse_iterator &other) const noexcept
    {
      return index == other.index;
    }

    bool operator!=(const reverse_iterator &other) const noexcept
    {
      return !(*this == other);
    }

    reference operator*() const noexcept
    {
      return array[index];
    }
  };

  /**
   * @brief Get an reverse_iterator to the beginning of the array
   *
   * @return reverse_iterator The reverse_iterator to the beginning of the array
   *
   * # Example
   * ```cpp
   * auto arr = tenno::array<int, 5>();
   * auto begin = arr.begin();
   * ```
   */
  reverse_iterator rbegin() noexcept
  {
    return reverse_iterator(*this, this->_size - 1);
  }
  /**
   * @brief Get an reverse_iterator to the end of the array
   *
   * @return reverse_iterator The reverse_iterator to the end of the array
   *
   * # Example
   * ```cpp
   * tenno::for_each(arr.begin(), arr.end(), [](int& elem) { elem = 0; });
   * ```
   */
  reverse_iterator rend() noexcept
  {
    return reverse_iterator(*this, -1);
  }

  /**
   * @brief A const iterator to iterate over the array
   */
  struct const_reverse_iterator
  {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;

    const tenno::array<T, N> &array;
    tenno::size index;

    constexpr explicit const_reverse_iterator(
      const tenno::array<T, N> &array_in, const tenno::size end) noexcept
        : array(array_in), index(end)
    {
    }

    constexpr const_reverse_iterator &operator++() noexcept
    {
      --index;
      return *this;
    }

    constexpr const_reverse_iterator operator++(int) noexcept
    {
      const_reverse_iterator _const_reverse_iterator = *this;
      --index;
      return _const_reverse_iterator;
    }

    constexpr bool
    operator==(const const_reverse_iterator &other) const noexcept
    {
      return index == other.index;
    }

    constexpr bool
    operator!=(const const_reverse_iterator &other) const noexcept
    {
      return !(*this == other);
    }

    constexpr T operator*() const noexcept
    {
      return array[index];
    }
  };

  /**
   * @brief Get an const_reverse_iterator to the beginning of the array
   *
   * @return const_reverse_iterator The iterator to the beginning of the array
   *
   * # Example
   * ```cpp
   * auto arr = tenno::array<int, 5>();
   * auto begin = arr.begin();
   * ```
   */
  constexpr const_reverse_iterator crbegin() const noexcept
  {
    return const_reverse_iterator(*this, this->_size - 1);
  }
  /**
   * @brief Get an const_reverse_iterator to the end of the array
   *
   * @return const_reverse_iterator The iterator to the end of the array
   *
   * # Example
   * ```cpp
   * tenno::for_each(arr.begin(), arr.end(), [](int& elem) { elem = 0; });
   * ```
   */
  constexpr const_reverse_iterator crend() const noexcept
  {
    return const_reverse_iterator(*this, -1);
  }

private:
  T _data[N];
  const tenno::size _size = N;

}; // class array

} // namespace tenno
