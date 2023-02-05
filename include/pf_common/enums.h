/**
 * @file enums.h
 * @brief Common enums and utility for working with enums.
 * @author Petr Flajšingr
 * @date 23.1.21
 */
#ifndef PF_COMMON_ENUMS_H
#define PF_COMMON_ENUMS_H

#include <algorithm>
#include <concepts>
#include <magic_enum.hpp>
#include <pf_common/concepts/Enum.h>
#include <pf_common/concepts/ranges.h>
#include <ranges>
#include <type_traits>

namespace pf {

/**
 * Common enum Enabled.
 */
enum class Enabled { Yes, No };

/**
 * Check enum values so that there is no bit overlap.
 * @tparam E type to check
 * @return true if no values overlap, false otherwise
 */
template<Enum E>
consteval bool IsPureFlags() {
  using UnderlyingType = std::underlying_type_t<E>;
  auto total = UnderlyingType{};
  for (auto e : magic_enum::enum_values<E>()) {
    if ((total & static_cast<UnderlyingType>(e)) != UnderlyingType{}) { return false; }
    total |= static_cast<UnderlyingType>(e);
  }
  return true;
}

/**
 * @brief Utility for using enums as flags.
 *
 * Inner value is not necessarily a valid value of E.
 *
 * @tparam E enum
 * @tparam DefaultValue value after construction
 */
template<Enum E>
class Flags {
 public:
  using UnderlyingType = std::underlying_type_t<E>;
  using EnumType = E;
  /**
   * Create Flags from multiple enum values
   * @param args enum values
   */
  constexpr explicit Flags(std::same_as<E> auto... args) { (operator|=(args), ...); }
  /**
   * Create Flags from range of enum values.
   * @param values enum values
   */
  constexpr explicit Flags(std::ranges::range auto &&values)
    requires(std::same_as<E, std::ranges::range_value_t<decltype(values)>>)
  {
    std::ranges::for_each(values, [this](auto val) { operator|=(val); });
  }
  /**
   * Copy assignment
   * @param other
   */
  constexpr Flags(const Flags &other) { value = other.value; }
  /**
   * Assignment form enum type. Overwrites inner state.
   * @param other new value
   */
  constexpr Flags(E other) { value = static_cast<UnderlyingType>(other); }
  /**
   * Copy assignment
   * @param other
   */
  constexpr Flags &operator=(const Flags &other) {
    value = other.value;
    return *this;
  }
  /**
   * Assignment form enum type. Overwrites inner state.
   * @param other new value
   */
  constexpr Flags &operator=(E other) {
    value = other;
    return *this;
  }
  /**
   * Flip flags.
   */
  [[nodiscard]] constexpr Flags operator~() {
    auto result = *this;
    result.value = ~value;
    return result;
  }
  [[nodiscard]] constexpr bool operator==(const Flags &rhs) const { return value == rhs.value; }
  [[nodiscard]] constexpr bool operator!=(const Flags &rhs) const { return !(rhs == *this); }
  [[nodiscard]] constexpr bool operator==(E rhs) const { return value == rhs; }
  [[nodiscard]] constexpr bool operator!=(E rhs) const { return !(rhs == *this); }

  constexpr Flags &operator|=(const Flags &other) {
    value = value | other.value;
    return *this;
  }
  [[nodiscard]] constexpr Flags operator|(const Flags &other) const {
    auto result = *this;
    result |= other;
    return result;
  }
  constexpr Flags &operator|=(E other) {
    value = value | static_cast<UnderlyingType>(other);
    return *this;
  }
  [[nodiscard]] constexpr Flags operator|(E other) const {
    auto result = *this;
    result |= other;
    return result;
  }
  constexpr Flags &operator&=(const Flags &other) {
    value = value & other.value;
    return *this;
  }
  [[nodiscard]] constexpr Flags operator&(const Flags &other) const {
    auto result = *this;
    result &= other;
    return result;
  }
  constexpr Flags &operator&=(E other) {
    value = value & static_cast<UnderlyingType>(other);
    return *this;
  }
  [[nodiscard]] constexpr Flags operator&(E other) const {
    auto result = *this;
    result &= other;
    return result;
  }
  constexpr Flags &operator^=(const Flags &other) {
    value = value ^ other.value;
    return *this;
  }
  [[nodiscard]] constexpr Flags operator^(const Flags &other) const {
    auto result = *this;
    result ^= other;
    return result;
  }
  constexpr Flags &operator^=(E other) {
    value = value ^ static_cast<UnderlyingType>(other);
    return *this;
  }
  [[nodiscard]] constexpr Flags operator^(E other) const {
    auto result = *this;
    result ^= other;
    return result;
  }

  /**
   * Check if the flag is contained within this object.
   * @param other flag to check
   * @return true if the value is contained in flags
   */
  [[nodiscard]] constexpr bool operator&&(E other) { return ((*this) & other).is(other); }
  /**
   * Check if the flag is contained within this object.
   * @param other flag to check
   * @return true if the value is contained in flags
   */
  [[nodiscard]] constexpr bool is(E other) const {
    const auto bitAnd = (value & static_cast<UnderlyingType>(other));
    return bitAnd == static_cast<UnderlyingType>(other);
  }

  /**
   * Get underlying value.
   * @return underlying value
   */
  [[nodiscard]] constexpr UnderlyingType operator*() const { return value; }

  /**
   * List all flags which are currently set.
   * @return flags which are currently set
   */
  [[nodiscard]] constexpr RangeOf<E> auto getSetFlags() const {
    return magic_enum::enum_values<E>() | std::views::filter([this](const auto val) { return is(val); });
  }

 private:
  UnderlyingType value{};
};

namespace enum_operators {
/**
 * Create Flags from two enums.
 * @tparam E enum type
 * @param lhs
 * @param rhs
 * @return
 */
template<Enum E>
constexpr Flags<E> operator|(E lhs, E rhs) {
  return Flags<E>{lhs, rhs};
}
/**
 * Create Flags from two enums.
 * @tparam E enum type
 * @param lhs
 * @param rhs
 * @return
 */
template<Enum E>
constexpr Flags<E> operator&(E lhs, E rhs) {
  auto result = Flags<E>{lhs};
  result &= rhs;
  return result;
}
/**
 * Bitwise negation. Resulting value might not be a valid value of E.
 * @tparam E type of enum
 * @param value value to negate
 * @return bitwise negated value
 */
template<Enum E>
constexpr E operator~(E value) {
  using UnderlyingType = std::underlying_type_t<E>;
  return static_cast<E>(~static_cast<UnderlyingType>(value));
}
}// namespace enum_operators

}// namespace pf

#ifdef PF_ENUM_OUT
inline std::ostream &operator<<(std::ostream &o, pf::Enum auto e) {
  o << magic_enum::enum_name(e);
  return o;
}
#endif

#define ENABLE_PF_ENUM_OUT_FOR_NAMESPACE(ns_name)                                                                                          \
  namespace ns_name {                                                                                                                      \
  inline std::ostream &operator<<(std::ostream &o, pf::Enum auto e) {                                                                      \
    o << magic_enum::enum_name(e);                                                                                                         \
    return o;                                                                                                                              \
  }                                                                                                                                        \
  }

#endif//PF_COMMON_ENUMS_H
