/**
 * @file references.h
 * @brief Concepts for references.
 * @author Petr Flajšingr
 * @date 30.1.23
 */

#ifndef PF_COMMON_REFERENCES_H
#define PF_COMMON_REFERENCES_H

#include <type_traits>

namespace pf {

template<typename T>
constexpr bool IsConstReference = std::is_reference_v<T> && std::is_const_v<std::remove_reference_t<T>>;
template<typename T>
constexpr bool IsPointerToConst = std::is_pointer_v<T> && std::is_const_v<std::remove_pointer_t<T>>;

}

#endif//PF_COMMON_REFERENCES_H
