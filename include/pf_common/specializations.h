/**
* @file specializations.h
* @brief Concepts for checking if a type is specialization of another.
* @author Petr Flajšingr
* @date 12.4.21
*/
#ifndef PF_COMMON_SPECIALIZATIONS_H
#define PF_COMMON_SPECIALIZATIONS_H

#include <type_traits>

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
  explicit conversion_tester(const F<T> &) {}
};
}// namespace detail
namespace details {
template<typename Derived, template<typename...> typename Base>
struct is_derived_specialization {
  using U = typename std::remove_cv<typename std::remove_reference<Derived>::type>::type;

  template<typename... Args>
  static auto test(Base<Args...> *) -> typename std::integral_constant<bool, !std::is_same<U, Base<Args...>>::value>;

  static std::false_type test(void *);

  using type = decltype(test(std::declval<U *>()));
};
}// namespace details

template<typename Derived, template<typename...> typename Base>
using is_derived_specialization = typename details::is_derived_specialization<Derived, Base>::type;

template<typename Derived, template<typename...> typename Base>
constexpr static bool is_derived_specialization_v = is_derived_specialization<Derived, Base>::value;

/**
* @brief Check if T is a direct or derived specialization of U.
*
* For example static_asssert(derived_specialization_of<DerivedFromStdVector<bool>, std::vector)
* @tparam T
* @tparam U
*/
template<typename Derived, template<typename...> typename Base>
concept derived_specialization_of = is_derived_specialization_v<Derived, Base>;

}// namespace pf

#endif//PF_COMMON_SPECIALIZATIONS_H
