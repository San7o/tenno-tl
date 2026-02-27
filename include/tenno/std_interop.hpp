// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <vector>
#include "vector.hpp"

namespace tenno
{

  /**
   * @brief Converts a std::vector to a tenno::vector (Copy)
   */
  template <typename T>
  tenno::vector<T> from_std(const std::vector<T>& std_vec)
  {
    tenno::vector<T> result;
    result.reserve(std_vec.size());
    for (const auto& item : std_vec)
    {
      result.push_back(item);
    }
    return result;
  }
  
  /**
   * @brief Converts a std::vector to a tenno::vector (Move)
   */
  template <typename T>
  tenno::vector<T> from_std(std::vector<T>&& std_vec)
  {
    tenno::vector<T> result;
    result.reserve(std_vec.size());
    for (auto& item : std_vec)
    {
        result.push_back(std::move(item));
    }
    std_vec.clear();
    return result;
  }

  /**
   * @brief Converts a tenno::vector to a std::vector (Copy)
   */
  template <typename T>
  std::vector<T> to_std(const tenno::vector<T>& tenno_vec)
  {
    std::vector<T> result;
    result.reserve(tenno_vec.size());
    for (size_t i = 0; i < tenno_vec.size(); ++i)
    {
      result.push_back(tenno_vec[i]);
    }
    return result;
  }

  /**
   * @brief Converts a tenno::vector to a std::vector (Move)
   */
  template <typename T>
  std::vector<T> to_std(tenno::vector<T>&& tenno_vec)
  {
    std::vector<T> result;
    result.reserve(tenno_vec.size());
    for (size_t i = 0; i < tenno_vec.size(); ++i)
    {
      result.push_back(std::move(tenno_vec[i]));
    }
    tenno_vec.clear(); 
    return result;
  }
  
} // namespace tenno
