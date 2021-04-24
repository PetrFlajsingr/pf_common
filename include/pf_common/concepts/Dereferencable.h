//
// Created by petr on 4/24/21.
//

#ifndef PF_COMMON_INCLUDE_PF_COMMON_CONCEPTS_DEREFERENCABLE_H
#define PF_COMMON_INCLUDE_PF_COMMON_CONCEPTS_DEREFERENCABLE_H

#include <pf_common/concepts/Negations.h>

namespace pf {
template <typename T>
concept Dereferencable = requires(T t) {
  {*t} -> NotSameAs<void>;
};

template<Dereferencable T>
using DereferencedType = decltype(*std::declval<T>());
}

#endif//PF_COMMON_INCLUDE_PF_COMMON_CONCEPTS_DEREFERENCABLE_H
