//
// Created by petr on 11/1/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UTILS_MATH_H
#define REALISTIC_VOXEL_RENDERING_UTILS_MATH_H

#include <compare>

namespace pf::math {

template<typename T>
concept RangeValueType = std::equality_comparable<T> &&requires(T t) {
  { t - t }
  ->std::convertible_to<T>;
}
&&std::convertible_to<T, std::size_t>;

template<RangeValueType T>
struct Range {
  T start;
  T end;

  constexpr bool operator==(const Range &rhs) const {
    return (*this <=> rhs) == std::strong_ordering::equal;
  }
  constexpr bool operator!=(const Range &rhs) const { return !(rhs == *this); }

  constexpr std::strong_ordering operator<=>(const Range &rhs) const {
    if (start == rhs.start && end == rhs.end) { return std::strong_ordering::equal; }
    const auto thisSize = getSize();
    const auto rhsSize = rhs.getSize();
    if (thisSize - rhsSize > 0) { return std::strong_ordering::greater; };
    return std::strong_ordering::less;
  }

  T getSize() const { return end - start; }
};
}// namespace pf_common::math

#endif//REALISTIC_VOXEL_RENDERING_UTILS_MATH_H
