/**
 * @file PointerLike.h
 * @brief Concept checking for pointer like object.
 * @author Petr Flajšingr
 * @date 2.9.22
 */

#ifndef PF_COMMON_POINTERLIKE_H
#define PF_COMMON_POINTERLIKE_H

#include <concepts>
#include <memory>
#include <type_traits>

namespace pf {

template<typename T>
concept PointerLike = std::pointer_traits<T>::element_type
    && requires(T t) {
         { *t } -> std::convertible_to<typename std::pointer_traits<T>::element_type>;
         { t.operator->() } -> std::same_as<std::add_pointer<typename std::pointer_traits<T>::element_type>>;
       };

}

#endif//PF_COMMON_POINTERLIKE_H
