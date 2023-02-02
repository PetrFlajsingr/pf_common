/**
 * @file algorithms.h
 * @brief Convenience functions similar to those in <algortihm>.
 * @author Petr Flajšingr
 * @date 26.10.20
 */
#ifndef PF_COMMON_ALGORITHMS_H
#define PF_COMMON_ALGORITHMS_H

#include <algorithm>
#include <concepts>
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
template<std::ranges::range Range, typename Proj = std::identity>
[[nodiscard]] constexpr bool contains(Range &&haystack, const auto &needle, Proj proj = {}) requires (std::equality_comparable_with<decltype(needle), std::ranges::range_value_t<Range>>) {
  return std::ranges::any_of(haystack, [&](auto &&value) { return needle == proj(value); });
}

/**
 * Find first common element of two ranges.
 * @param range1
 * @param range2
 * @return first common element if one is found, std::nullopt otherwise
 */
[[nodiscard]] constexpr auto findFirstCommon(const std::ranges::range auto &range1, const std::ranges::range auto &range2) {
  static_assert(std::equality_comparable_with<std::ranges::range_value_t<decltype(range1)>, std::ranges::range_value_t<decltype(range2)>>);
  using ResultType = std::ranges::range_value_t<decltype(range1)>;
  if (auto iter = std::ranges::find_if(range1, [&range2](const auto &val) { return contains(range2, val); }); iter != range1.end()) {
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
template<std::ranges::range Range>
std::optional<std::ranges::range_value_t<Range>> findIf(Range &&range, std::predicate<std::ranges::range_value_t<Range>> auto &&predicate) {
  if (const auto iter = std::ranges::find_if(range, predicate); iter != std::ranges::end(range)) { return *iter; }
  return std::nullopt;
}

}// namespace pf
#endif//PF_COMMON_ALGORITHMS_H
