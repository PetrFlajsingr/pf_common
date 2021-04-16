//
// Created by petr on 10/26/20.
//

#ifndef PF_COMMON_ALGORITHMS_H
#define PF_COMMON_ALGORITHMS_H

#include <algorithm>
#include <optional>
#include <range/v3/range/concepts.hpp>
#include <range/v3/range/traits.hpp>
#include <ranges>

namespace pf {
/**
 * Check if a range contains given value
 * @param needle value searched for
 * @param haystack range to search through
 * @return true if the value is found, false otherwise
 */
template<std::ranges::range Range>
[[nodiscard]] bool isIn(const auto &needle, Range &&haystack) {
  static_assert(std::equality_comparable_with<decltype(needle), std::ranges::range_value_t<Range>>);
  return std::ranges::any_of(haystack, [&needle](const auto &value) { return needle == value; });
}

/**
 * Find first common element of two ranges.
 * @param range1
 * @param range2
 * @return first common element if one is found, std::nullopt otherwise
 */
[[nodiscard]] auto findFirstCommon(const std::ranges::range auto &range1, const std::ranges::range auto &range2) {
  static_assert(std::equality_comparable_with<std::ranges::range_value_t<decltype(range1)>,
                                              std::ranges::range_value_t<decltype(range2)>>);
  using ResultType = std::ranges::range_value_t<decltype(range1)>;
  if (auto iter = std::ranges::find_if(range1, [&range2](const auto &val) { return isIn(val, range2); });
      iter != range1.end()) {
    return std::optional<ResultType>{*iter};
  }
  return std::optional<ResultType>{};
}

/**
 * Convenience function similar to std::find_if
 * @param range range to search through
 * @param predicate predicate returning true on desired element
 * @return element for which predicate returns true or std::nullopt if no such element is found
 */
template<ranges::range Range>
std::optional<ranges::range_value_t<Range>> findIf(Range &&range,
                                                   std::predicate<ranges::range_value_t<Range>> auto &&predicate) {
  if (const auto iter = std::ranges::find_if(range, predicate); iter != ranges::end(range)) { return *iter; }
  return std::nullopt;
}

}// namespace pf
#endif//PF_COMMON_ALGORITHMS_H
