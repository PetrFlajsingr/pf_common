/**
 * @file Integral.h
 * @brief Concept to check for integral type.
 * @author Petr Flajšingr
 * @date 2.4.22
 */

#ifndef PF_COMMON_CONCEPTS_INTEGRAL_H
#define PF_COMMON_CONCEPTS_INTEGRAL_H

#include <type_traits>
#include "OneOf.h"

namespace pf {

template<typename T>
concept Integral = std::is_integral_v<T>;

template<typename T>
concept CharType = OneOf<T, unsigned char, char, char8_t, char16_t, char32_t, wchar_t>;

}

#endif//PF_COMMON_CONCEPTS_INTEGRAL_H
