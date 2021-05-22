//
// Created by petr on 11/1/20.
//
/**
 * @file Range.h
 * @brief Range on a simple axis.
 * @author Petr Flajšingr
 * @date 1.11.20
 */
#ifndef PF_COMMON_MATH_H
#define PF_COMMON_MATH_H

#include <compare>

namespace pf::math {
/**
 * @brief Type usable for a range.
 * @tparam T
 */
template<typename T>
concept RangeValueType = std::equality_comparable<T> && requires(T t) {
  { t - t } -> std::convertible_to<T>;
};

/**
 * @brief 1D range.
 * @tparam T underlying type
 */
template<RangeValueType T>
struct Range {
  T start; /**< Starting position. */
  T end; /**< Ending position. */

  constexpr bool operator==(const Range &rhs) const { return start == rhs.start && end == rhs.end; }
  constexpr bool operator!=(const Range &rhs) const { return !(rhs == *this); }

  /**
   * Width of the range.
   * @return width of the range
   */
  [[nodiscard]] constexpr T getSize() const { return end - start; }
  /**
   * Check if a value is contained within the range.
   * @param val checked value
   * @return true if the value is contained within the range, false otherwise
   */
  [[nodiscard]] constexpr bool contains(T val) { return start <= val && val <= end; }
};
}// namespace pf::math

#endif//PF_COMMON_MATH_H
