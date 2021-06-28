/**
 * @file OneOf.h
 * @brief Static checking of types in type lists and values in value lists.
 * @author Petr Flajšingr
 * @date 19.10.20
 */
#ifndef PF_COMMON_ONEOF_H
#define PF_COMMON_ONEOF_H

#include <concepts>
namespace pf {
/**
 * @brief Check if T is one of types provided in Args...
 * @tparam T needle
 * @tparam Args haystack
 */
template<typename T, typename... Args>
constexpr bool OneOf_v = (std::same_as<T, Args> || ...);

/**
 * @brief A type which is one of Args...
 * @tparam T needle
 * @tparam Args haystack
 */
template<typename T, typename... Args>
concept OneOf = (std::same_as<T, Args> || ...);

template <auto Value, decltype(Value) ...Values>
constexpr bool OneOfValues_v = ((Value == Values) || ...);
}// namespace pf
#endif//PF_COMMON_ONEOF_H
