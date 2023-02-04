//
// Created by xflajs00 on 04.02.2023.
//

#ifndef PF_COMMON_ENUM_H
#define PF_COMMON_ENUM_H

#include <concepts>
#include <type_traits>

namespace pf {

/**
 * @brief Detect enum class.
 * @tparam T
 */
template<typename T>
concept ScopedEnum = std::is_enum_v<T> && !std::convertible_to<T, typename std::underlying_type_t<T>>;

/**
 * Detect old style enum.
 * @tparam T
 */
template<typename T>
concept UnscopedEnum = std::is_enum_v<T> && std::convertible_to<T, typename std::underlying_type_t<T>>;

/**
 * @brief enum or enum class
 * @tparam T
 */
template<typename T>
concept Enum = std::is_enum_v<T>;

}// namespace pf

#endif//PF_COMMON_ENUM_H
