//
// Created by petr on 4/24/21.
//

#ifndef PF_COMMON_INCLUDE_PF_COMMON_CONCEPTS_NEGATIONS_H
#define PF_COMMON_INCLUDE_PF_COMMON_CONCEPTS_NEGATIONS_H

#include<concepts>
namespace pf {

template <typename T, typename U>
concept NotSameAs = !std::same_as<T, U>;

}

#endif//PF_COMMON_INCLUDE_PF_COMMON_CONCEPTS_NEGATIONS_H
