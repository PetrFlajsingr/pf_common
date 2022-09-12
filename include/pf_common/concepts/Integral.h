/**
 * @file Integral.h
 * @brief Concept to check for integral type.
 * @author Petr Flajšingr
 * @date 2.4.22
 */

#ifndef PF_COMMON_INTEGRAL_H
#define PF_COMMON_INTEGRAL_H

#include <type_traits>

namespace pf {
template<typename T>
concept Integral = std::is_integral_v<T>;
}

#endif//PF_COMMON_INTEGRAL_H
