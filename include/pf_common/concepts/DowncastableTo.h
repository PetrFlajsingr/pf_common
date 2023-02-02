/**
 * @file DowncastableTo.h
 * @brief Concepts for checking downcast capabilities.
 * @author Petr Flajšingr
 * @date 30.1.23
 */

#ifndef PF_COMMON_CONCEPTS_DOWNCASTABLETO_H
#define PF_COMMON_CONCEPTS_DOWNCASTABLETO_H

#include <concepts>
#include <pf_common/concepts/references.h>

namespace pf {

/**
 * Check if parameter Base is a reference downcastable to Derived.
 */
template<typename Derived, typename Base>
concept DowncastableReferenceTo = std::is_reference_v<Derived> && std::is_reference_v<Base>
    && (IsConstReference<Base> && IsConstReference<Derived>
        || !IsConstReference<Base>) &&std::derived_from<std::remove_reference_t<Derived>, std::remove_reference_t<Base>>;
/**
 * Check if parameter Base is a pointer downcastable to Derived.
 */
template<typename Derived, typename Base>
concept DowncastablePointerTo = std::is_pointer_v<Derived> && std::is_pointer_v<Base>
    && (IsPointerToConst<Base> && IsPointerToConst<Derived>
        || !IsPointerToConst<Base>) &&std::derived_from<std::remove_pointer_t<Derived>, std::remove_pointer_t<Base>>;
/**
 * Check if parameter Base is a reference or a pointer downcastable to Derived.
 */
template<typename Derived, typename Base>
concept DowncastableTo = DowncastableReferenceTo<Derived, Base> || DowncastablePointerTo<Derived, Base>;
}// namespace pf

#endif//PF_COMMON_CONCEPTS_DOWNCASTABLETO_H
