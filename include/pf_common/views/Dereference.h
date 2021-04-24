//
// Created by petr on 4/24/21.
//

#ifndef PF_COMMON_INCLUDE_PF_COMMON_VIEWS_DEREFERENCE_H
#define PF_COMMON_INCLUDE_PF_COMMON_VIEWS_DEREFERENCE_H

#include <concepts>
#include <pf_common/concepts/Dereferencable.h>
#include <ranges>

namespace pf::views {

namespace detail {
constexpr static inline auto dereferenceFnc = [](auto &t) -> DereferencedType<decltype(t)> { return *t; };
}

inline constexpr auto Dereference =
    []<std::ranges::viewable_range Range>(Range && r) requires(Dereferencable<std::ranges::range_value_t<Range>>) {
  return std::views::transform(std::forward<Range>(r), detail::dereferenceFnc);
};

template<std::ranges::viewable_range Range>
constexpr auto operator|(Range &&r, const decltype(Dereference) &o) {
  return o(std::forward<Range>(r));
}

}// namespace pf::views

#endif//PF_COMMON_INCLUDE_PF_COMMON_VIEWS_DEREFERENCE_H
