//
// Created by xflajs00 on 02.04.2022.
//

#ifndef PF_COMMON_INTEGRAL_H
#define PF_COMMON_INTEGRAL_H

#include <type_traits>

namespace pf {
template<typename T>
concept Integral = std::is_integral_v<T>;
}

#endif//PF_COMMON_INTEGRAL_H
