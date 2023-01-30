/**
 * @file casts.h
 * @brief Various cast functions
 * @author Petr Flajšingr
 * @date 30.1.23
 */

#ifndef PF_COMMON_CASTS_H
#define PF_COMMON_CASTS_H

#include <pf_common/concepts/downcast.h>

// Narrowing cast error
#if !defined(PF_NARROW_FAIL)
#include <cassert>
#define PF_NARROW_FAIL(msg) assert(false)
#endif

// Polymorphic downcast error
#if !defined(PF_POLYMORHPHIC_DOWNCAST_FAIL)
#if defined(NDEBUG)
#define PF_POLYMORHPHIC_DOWNCAST_FAIL(msg)
#else
#include <cassert>
#define PF_POLYMORHPHIC_DOWNCAST_FAIL(msg) assert(false)
#endif
#endif

#if !defined(PF_POLYMORHPHIC_DOWNCAST_CHECK_ENABLE)
#if defined(NDEBUG)
#define PF_POLYMORHPHIC_DOWNCAST_CHECK_ENABLE false
#else
#define PF_POLYMORHPHIC_DOWNCAST_CHECK_ENABLE true
#endif
#else
static_assert(std::same_as<bool, decltype(PF_POLYMORHPHIC_DOWNCAST_CHECK_ENABLE)>,
              "PF_POLYMORHPHIC_DOWNCAST_CHECK_ENABLE has to be a bool");
#endif

#include <fmt/core.h>

namespace pf {
/**
 * Narrowing cast, behaves the same as static_cast.
 */
template<typename To, typename From>
  requires std::convertible_to<From, To> && std::is_arithmetic_v<From> && std::is_arithmetic_v<To>
[[nodiscard]] constexpr To narrow_cast(From src) {
  return static_cast<From>(src);
}
/**
 * Narrowing cast, which fails if the narrowing changes the value.
 */
template<typename To, typename From>
  requires std::convertible_to<From, To> && std::is_arithmetic_v<From> && std::is_arithmetic_v<To>
[[nodiscard]] constexpr To narrow(From src) {
  constexpr bool isSameSign = std::is_signed_v<From> == std::is_signed_v<To>;

  const auto result = narrow_cast<To>(src);

  if (static_cast<From>(result) != src || (!isSameSign && ((result < To{}) != (src < From{})))) {
    PF_NARROW_FAIL(fmt::format("Checked narrow cast failed, source: {}, result: {}", src, result));
  }
  return result;
}

namespace details {
template<typename Derived, typename Base>
struct DynamicCastChecker {
  [[nodiscard]] constexpr bool operator()(Base base) { return dynamic_cast<Derived>(base) != nullptr; }
};

}// namespace details



template<typename Derived, typename Base, typename Checker = details::DynamicCastChecker<Derived, Base>>
  requires DowncastableTo<Derived, Base> && std::is_invocable_r_v<bool, Checker, Base>
    && std::is_default_constructible_v<Checker>
[[nodiscard]] constexpr Derived polymorphic_downcast(Base base) {
#if PF_POLYMORHPHIC_DOWNCAST_CHECK_ENABLE
  if (!Checker{}(base)) { PF_POLYMORHPHIC_DOWNCAST_FAIL("Invalid cast"); }
#endif
  return static_cast<Derived>(base);
}

#undef PF_NARROW_FAIL
#undef PF_POLYMORHPHIC_DOWNCAST_FAIL
#undef PF_POLYMORHPHIC_DOWNCAST_CHECK_ENABLE

}// namespace pf

#endif//PF_COMMON_CASTS_H
