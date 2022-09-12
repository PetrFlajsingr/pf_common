/**
 * @file CommonType.h
 * @brief Concept to check if all types have a common type.
 * @author Petr Flajšingr
 * @date 22.4.21
 */

#ifndef PF_COMMON_CONCEPTS_COMMONTYPE_H
#define PF_COMMON_CONCEPTS_COMMONTYPE_H

#include <concepts>

namespace pf {
template<typename T, typename... Args>
concept HaveCommonType = (std::common_with<T, Args> && ...);
}// namespace pf

#endif//PF_COMMON_CONCEPTS_COMMONTYPE_H
