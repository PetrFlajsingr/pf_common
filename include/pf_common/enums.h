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
#include <ranges>
#include <type_traits>

namespace pf {

/**
 * Common enum Enabled.
 */
enum class Enabled { Yes, No };

/**
 * @brief Detect enum class.
 * @tparam T
 */
template<typename T>
concept ScopedEnum = std::is_enum_v<T> && !std::convertible_to<T, typename std::underlying_type_t<T>>;

/**
 * @brief enum or enum class
 * @tparam T
 */
template<typename T>
concept Enum = std::is_enum_v<T> || ScopedEnum<T>;

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
  explicit Flags(std::same_as<E> auto... args) { (operator|=(args), ...); }
  /**
   * Create Flags from range of enum values.
   * @param values enum values
   */
  explicit Flags(const std::ranges::range auto &values) requires(
      std::same_as<E, std::ranges::range_value_t<decltype(values)>>) {
    std::ranges::for_each(values, [this](auto val) { operator|=(val); });
  } /**
   * Copy assignment
   * @param other
   */
  Flags(const Flags &other) { value = other.value; }
  /**
   * Assignment form enum type. Overwrites inner state.
   * @param other new value
   */
  Flags(E other) { value = other; }
  /**
   * Copy assignment
   * @param other
   */
  Flags &operator=(const Flags &other) {
    value = other.value;
    return *this;
  }
  /**
   * Assignment form enum type. Overwrites inner state.
   * @param other new value
   */
  Flags &operator=(E other) {
    value = other;
    return *this;
  }
  /**
   * Flip flags.
   */
  [[nodiscard]] Flags operator~() {
    auto result = *this;
    result.value = static_cast<E>(~static_cast<UnderlyingType>(value));
    return result;
  }
  [[nodiscard]] bool operator==(const Flags &rhs) const { return value == rhs.value; }
  [[nodiscard]] bool operator!=(const Flags &rhs) const { return !(rhs == *this); }
  [[nodiscard]] bool operator==(E rhs) const { return value == rhs; }
  [[nodiscard]] bool operator!=(E rhs) const { return !(rhs == *this); }
  Flags &operator|=(const Flags &other) {
    value = static_cast<E>(static_cast<UnderlyingType>(value) | static_cast<UnderlyingType>(other.value));
    return *this;
  }
  [[nodiscard]] Flags operator|(const Flags &other) const {
    auto result = *this;
    result |= other;
    return result;
  }
  Flags &operator|=(E other) {
    value = static_cast<E>(static_cast<UnderlyingType>(value) | static_cast<UnderlyingType>(other));
    return *this;
  }
  [[nodiscard]] Flags operator|(E other) const {
    auto result = *this;
    result |= other;
    return result;
  }
  Flags &operator&=(const Flags &other) {
    value = static_cast<E>(static_cast<UnderlyingType>(value) & static_cast<UnderlyingType>(other.value));
    return *this;
  }
  [[nodiscard]] Flags operator&(const Flags &other) const {
    auto result = *this;
    result &= other;
    return result;
  }
  Flags &operator&=(E other) {
    value = static_cast<E>(static_cast<UnderlyingType>(value) & static_cast<UnderlyingType>(other));
    return *this;
  }
  [[nodiscard]] Flags operator&(E other) const {
    auto result = *this;
    result &= other;
    return result;
  }
  Flags &operator^=(const Flags &other) {
    value = static_cast<E>(static_cast<UnderlyingType>(value) ^ static_cast<UnderlyingType>(other.value));
    return *this;
  }
  [[nodiscard]] Flags operator^(const Flags &other) const {
    auto result = *this;
    result ^= other;
    return result;
  }
  Flags &operator^=(E other) {
    value = static_cast<E>(static_cast<UnderlyingType>(value) ^ static_cast<UnderlyingType>(other));
    return *this;
  }
  [[nodiscard]] Flags operator^(E other) const {
    auto result = *this;
    result ^= other;
    return result;
  }

  /**
   * Check if the flag is contained within this object.
   * @param other flag to check
   * @return true if the value is contained in flags
   */
  [[nodiscard]] bool operator&&(E other) { return ((*this) & other).is(other); }
  /**
   * Check if the flag is contained within this object.
   * @param other flag to check
   * @return true if the value is contained in flags
   */
  [[nodiscard]] bool is(E other) const {
    const auto bitAnd = (static_cast<UnderlyingType>(value) & static_cast<UnderlyingType>(other));
    return bitAnd == static_cast<UnderlyingType>(other);
  }

  /**
   * List all flags which are currently set.
   * @return flags which are currently set
   */
  [[nodiscard]] auto getSetFlags() const {
    return magic_enum::enum_values<E>() | std::views::filter([this](const auto val) { return is(val); });
  }

 private:
  E value = E{};
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
Flags<E> operator|(E lhs, E rhs) {
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
Flags<E> operator&(E lhs, E rhs) {
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
E operator~(E value) {
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

#endif//PF_COMMON_ENUMS_H
