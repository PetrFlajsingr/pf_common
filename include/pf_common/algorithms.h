//
// Created by petr on 10/26/20.
//

#ifndef PF_COMMON_ALGORITHMS_H
#define PF_COMMON_ALGORITHMS_H

#include <algorithm>
#include <optional>
#include <range/v3/range/concepts.hpp>
#include <range/v3/range/traits.hppt>

namespace pf {
template<typename T, typename Container = std::initializer_list<T>>
bool isIn(const T &val, Container &&vals) {
  return std::ranges::any_of(vals, [&val](const auto &v) { return val == v; });
}

template<typename T, template<class> typename Container, template<class> typename Container2>
std::optional<T> findFirstCommon(const Container<T> &vals, const Container2<T> &vals2) {
  for (const auto &val : vals) {
    if (isIn(val, vals2)) { return val; }
  }
  return std::nullopt;
}

template<ranges::range Range>
std::optional<ranges::range_value_t<Range>> findIf(Range &&range,
                                                   std::predicate<ranges::range_value_t<Range>> auto &&predicate) {
  if (const auto iter = std::ranges::find_if(range, predicate); iter != ranges::end(range)) { return *iter; }
  return std::nullopt;
}

}// namespace pf
#endif//PF_COMMON_ALGORITHMS_H
