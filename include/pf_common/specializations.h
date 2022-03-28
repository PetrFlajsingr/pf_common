/**
* @file specializations.h
* @brief Concepts for checking if a type is specialization of another.
* @author Petr Flajšingr
* @date 12.4.21
*/
#ifndef PF_COMMON_SPECIALIZATIONS_H
#define PF_COMMON_SPECIALIZATIONS_H

namespace pf {
template<class T, template<class...> class Template>
struct is_direct_specialization : std::false_type {};

template<template<class...> class Template, class... Args>
struct is_direct_specialization<Template<Args...>, Template> : std::true_type {};

template<typename T, template<class...> class U>
constexpr static auto is_direct_specialization_v = is_direct_specialization<T, U>::value;

/**
* @brief Check if T is a direct specialization of U.
*
* For example static_assert(direct_specialization_of<std::vector<bool>, std::vector)
* @tparam T
* @tparam U
*/
template<typename T, template<class...> class U>
concept direct_specialization_of = is_direct_specialization_v<T, U>;

namespace detail {
template<template<typename> class F>
struct conversion_tester {
 template<typename T>
 explicit conversion_tester(const F<T> &);
};
}// namespace detail

template<class From, template<typename> class To>
struct is_derived_specialization {
 static const bool value = std::is_convertible<From, detail::conversion_tester<To>>::value;
};

template<typename T, template<class...> class U>
constexpr static auto is_derived_specialization_v = is_derived_specialization<T, U>::value;
/**
* @brief Check if T is a direct or derived specialization of U.
*
* For example static_asssert(derived_specialization_of<DerivedFromStdVector<bool>, std::vector)
* @tparam T
* @tparam U
*/
template<typename T, template<class...> class U>
concept derived_specialization_of = is_derived_specialization_v<T, U>;

}// namespace pf

#endif//PF_COMMON_SPECIALIZATIONS_H
