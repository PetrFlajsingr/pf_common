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
template<std::ranges::range Range>
bool isIn(const auto &needle, Range &&haystack) {
  static_assert(std::equality_comparable_with<decltype(needle), std::ranges::range_value_t<Range>>);
  return std::ranges::any_of(haystack, [&needle](const auto &value) { return needle == value; });
}

auto findFirstCommon(const std::ranges::range auto &range1, const std::ranges::range auto &range2) {
  static_assert(std::equality_comparable_with<std::ranges::range_value_t<decltype(range1)>,
                                              std::ranges::range_value_t<decltype(range2)>>);
  using ResultType = std::ranges::range_value_t<decltype(range1)>;
  if (auto iter = std::ranges::find_if(range1, [&range2](const auto &val) { return isIn(val, range2); });
      iter != range1.end()) {
    return std::optional<ResultType>{*iter};
  }
  return std::optional<ResultType>{};
}

template<ranges::range Range>
std::optional<ranges::range_value_t<Range>> findIf(Range &&range,
                                                   std::predicate<ranges::range_value_t<Range>> auto &&predicate) {
  if (const auto iter = std::ranges::find_if(range, predicate); iter != ranges::end(range)) { return *iter; }
  return std::nullopt;
}

}// namespace pf
#endif//PF_COMMON_ALGORITHMS_H
