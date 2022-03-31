//
// Created by xflajs00 on 31.03.2022.
//

#ifndef PF_COMMON_RANGES_H
#define PF_COMMON_RANGES_H

#include <ranges>
#include <concepts>

namespace pf {
template<typename T, typename ElementType>
concept RangeOf = std::ranges::range<T> && std::same_as<std::ranges::range_value_t<T>, ElementType>;
}

#endif//PF_COMMON_RANGES_H
