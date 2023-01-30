//
// Created by xflajs00 on 30.01.2023.
//

#ifndef PF_COMMON_DOWNCAST_H
#define PF_COMMON_DOWNCAST_H

#include <concepts>
#include <pf_common/concepts/references.h>

namespace pf {

template<typename Derived, typename Base>
concept DowncastableReferenceTo = std::is_reference_v<Derived> && std::is_reference_v<Base>
    && (IsConstReference<Base> && IsConstReference<Derived>
        || !IsConstReference<Base>) &&std::derived_from<std::remove_reference_t<Derived>, std::remove_reference_t<Base>>;

template<typename Derived, typename Base>
concept DowncastablePointerTo = std::is_pointer_v<Derived> && std::is_pointer_v<Base>
    && (IsPointerToConst<Base> && IsPointerToConst<Derived>
        || !IsPointerToConst<Base>) &&std::derived_from<std::remove_pointer_t<Derived>, std::remove_pointer_t<Base>>;

template<typename Derived, typename Base>
concept DowncastableTo = DowncastableReferenceTo<Derived, Base> || DowncastablePointerTo<Derived, Base>;
}// namespace pf

#endif//PF_COMMON_DOWNCAST_H
