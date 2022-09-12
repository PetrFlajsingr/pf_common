/**
 * @file ranges.h
 * @brief Concepts for ranges.
 * @author Petr Flajšingr
 * @date 31.3.22
 */

#ifndef PF_COMMON_RANGES_H
#define PF_COMMON_RANGES_H

#include <ranges>
#include <concepts>

namespace pf {
/**
 * Range of given type
 * @tparam R range
 * @tparam ElementType value type
 */
template<typename R, typename ElementType>
concept RangeOf = std::ranges::range<R> && std::same_as<std::ranges::range_value_t<R>, ElementType>;
}

#endif//PF_COMMON_RANGES_H
