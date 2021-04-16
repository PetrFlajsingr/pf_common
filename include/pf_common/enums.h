//
// Created by petr on 1/23/21.
//

#ifndef PF_COMMON_INCLUDE_PF_COMMON_ENUMS_H
#define PF_COMMON_INCLUDE_PF_COMMON_ENUMS_H

#include <algorithm>
#include <concepts>
#include <ranges>
#include <type_traits>

namespace pf {

enum class Enabled { Yes, No };

#define ENABLE_BIT_MASK_ENUM(enum_type)                                                                                \
  inline enum_type operator|(enum_type lhs, enum_type rhs) {                                                           \
    using T = std::underlying_type_t<enum_type>;                                                                       \
    return static_cast<enum_type>(static_cast<T>(lhs) | static_cast<T>(rhs));                                          \
  }                                                                                                                    \
  inline enum_type &operator|=(enum_type &lhs, enum_type rhs) {                                                        \
    lhs = lhs | rhs;                                                                                                   \
    return lhs;                                                                                                        \
  }                                                                                                                    \
  inline bool is(enum_type self, enum_type other) {                                                                    \
    using T = std::underlying_type_t<enum_type>;                                                                       \
    return (static_cast<T>(self) & static_cast<T>(other)) != 0;                                                        \
  }                                                                                                                    \
  inline enum_type operator&(enum_type lhs, enum_type rhs) {                                                           \
    using T = std::underlying_type_t<enum_type>;                                                                       \
    return static_cast<enum_type>(static_cast<T>(lhs) | static_cast<T>(rhs));                                          \
  }                                                                                                                    \
  inline enum_type &operator&=(enum_type &lhs, enum_type rhs) {                                                        \
    lhs = lhs & rhs;                                                                                                   \
    return lhs;                                                                                                        \
  }

template<typename T>
concept ScopedEnum = std::is_enum_v<T> && !std::convertible_to<T, typename std::underlying_type_t<T>>;

template<typename T>
concept Enum = std::is_enum_v<T> || ScopedEnum<T>;

template<Enum E, E DefaultValue = E{}>
class Flags {
  using UnderlyingType = std::underlying_type_t<E>;

 public:
  explicit Flags(std::same_as<E> auto... args) {
    (operator|=(args), ...);
  }
  explicit Flags(const std::ranges::range auto &vals) requires(std::same_as<E, std::ranges::range_value_t<decltype(vals)>>) {
    std::ranges::for_each(vals, [this](auto val) {
      operator|=(val);
    });
  }
  Flags &operator=(const Flags &other) {
    value = other.value;
    return *this;
  }
  Flags &operator=(E other) {
    value = other;
    return *this;
  }
  [[nodiscard]] Flags operator~() {
    auto result = *this;
    result.value = static_cast<E>(~static_cast<UnderlyingType>(value));
    return result;
  }
  [[nodiscard]] bool operator==(const Flags &rhs) const {
    return value == rhs.value;
  }
  [[nodiscard]] bool operator!=(const Flags &rhs) const {
    return !(rhs == *this);
  }
  [[nodiscard]] bool operator==(E rhs) const {
    return value == rhs;
  }
  [[nodiscard]] bool operator!=(E rhs) const {
    return !(rhs == *this);
  }
  Flags &operator|=(const Flags &other) {
    value = static_cast<E>(static_cast<UnderlyingType>(value) | static_cast<UnderlyingType>(other.value));
    return *this;
  }
  [[nodiscard]] Flags operator|(const Flags &other) const {
    auto result = *this;
    result.value |= other;
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
    result.value &= other;
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
  Flags &operator^=(const Flags *other) {
    value = static_cast<E>(static_cast<UnderlyingType>(value) ^ static_cast<UnderlyingType>(other));
    return *this;
  }
  [[nodiscard]] Flags operator^(const Flags *other) const {
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

  [[nodiscard]] bool operator&&(E other) {
    return ((*this) & other).is(other);
  }

  [[nodiscard]] bool is(E other) const {
    const auto bitAnd = (static_cast<UnderlyingType>(value) & static_cast<UnderlyingType>(other));
    return bitAnd != 0 || bitAnd == static_cast<UnderlyingType>(other);
  }

 private:
  E value = DefaultValue;
};

namespace enum_flag_operators {
template<Enum E>
Flags<E> operator|(E lhs, E rhs) {
  return Flags<E>{lhs, rhs};
}

template<Enum E>
Flags<E> operator&(E lhs, E rhs) {
  auto result = Flags<E>{lhs};
  result &= rhs;
  return result;
}
}

}// namespace pf

#endif//PF_COMMON_INCLUDE_PF_COMMON_ENUMS_H
