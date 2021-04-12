//
// Created by petr on 4/12/21.
//

#ifndef PF_COMMON_INCLUDE_PF_COMMON_SPECIALIZATIONS_H
#define PF_COMMON_INCLUDE_PF_COMMON_SPECIALIZATIONS_H

namespace pf {
template<class T, template<class...> class Template>
struct is_specialization : std::false_type {};

template<template<class...> class Template, class... Args>
struct is_specialization<Template<Args...>, Template> : std::true_type {};

template<typename T, template<class...> class U>
constexpr static auto is_specialization_v = is_specialization<T, U>::value;

template<typename T, template<class...> class U>
concept specialization_of = is_specialization_v<T, U>;
}// namespace pf

#endif//PF_COMMON_INCLUDE_PF_COMMON_SPECIALIZATIONS_H
