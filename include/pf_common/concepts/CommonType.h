//
// Created by petr on 4/22/21.
//

#ifndef PF_COMMON_INCLUDE_PF_COMMON_CONCEPTS_COMMONTYPE_H
#define PF_COMMON_INCLUDE_PF_COMMON_CONCEPTS_COMMONTYPE_H

#include <concepts>

namespace pf {
template<typename T, typename... Args>
constexpr auto HaveCommonType = (std::common_with<T, Args> && ...);
}

#endif//PF_COMMON_INCLUDE_PF_COMMON_CONCEPTS_COMMONTYPE_H
