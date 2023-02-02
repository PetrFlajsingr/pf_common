/**
 * @file Integral.h
 * @brief Concept to check for integral type.
 * @author Petr Flajšingr
 * @date 2.4.22
 */

#ifndef PF_COMMON_CONCEPTS_FUNDAMENTAL_TYPES_H
#define PF_COMMON_CONCEPTS_FUNDAMENTAL_TYPES_H

#include "OneOf.h"
#include <type_traits>

namespace pf {

template<typename T>
concept Integral = std::is_integral_v<T>;

template<typename T>
concept CharType = OneOf<T, unsigned char, char, char8_t, char16_t, char32_t, wchar_t>;
// Not including bool
template<typename T>
concept IntType =
    OneOf<T, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t, std::int8_t, std::int16_t, std::int32_t, std::int64_t>;

template<typename T>
concept FloatType = std::is_floating_point_v<T>;

}// namespace pf

#endif//PF_COMMON_CONCEPTS_FUNDAMENTAL_TYPES_H
