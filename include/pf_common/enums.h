//
// Created by petr on 1/23/21.
//

#ifndef PF_COMMON_INCLUDE_PF_COMMON_ENUMS_H
#define PF_COMMON_INCLUDE_PF_COMMON_ENUMS_H

namespace pf {

enum class Enabled { Yes, No };

#define ENABLE_BIT_MASK_ENUM(enum_type)                                                                                \
  inline Stretch operator|(enum_type lhs, enum_type rhs) {                                                             \
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

}// namespace pf

#endif//PF_COMMON_INCLUDE_PF_COMMON_ENUMS_H
