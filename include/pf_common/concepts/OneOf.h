//
// Created by petr on 10/19/20.
//

#ifndef PF_COMMON_ONEOF_H
#define PF_COMMON_ONEOF_H

#include <concepts>
namespace pf {
template<typename T, typename... Args>
constexpr bool OneOf_v = (std::same_as<T, Args> || ...);

template<typename T, typename... Args>
concept OneOf = (std::same_as<T, Args> || ...);
}// namespace pf_common
#endif//PF_COMMON_ONEOF_H
