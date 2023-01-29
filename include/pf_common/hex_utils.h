//
// Created by xflajs00 on 29.01.2023.
//

#ifndef PF_COMMON_HEX_UTILS_H
#define PF_COMMON_HEX_UTILS_H

#include <pf_common/concepts/Integral.h>
#include <array>

namespace pf {

template<CharType T>
[[nodiscard]] constexpr std::uint8_t hexDigitToValue(const T ch) {
  if (ch >= static_cast<T>('0') && ch <= static_cast<T>('9')) {
    return static_cast<std::uint8_t>(ch - static_cast<T>('0'));
  }
  if (ch >= static_cast<T>('a') && ch <= static_cast<T>('f')) {
    return static_cast<std::uint8_t>(10 + ch - static_cast<T>('a'));
  }
  if (ch >= static_cast<T>('A') && ch <= static_cast<T>('F')) {
    return static_cast<std::uint8_t>(10 + ch - static_cast<T>('A'));
  }
  return 0;
}
template<CharType T>
[[nodiscard]] constexpr T valueToHexDigit(const std::uint8_t value) {
  //FIXME: check input validity - contracts?
  constexpr static auto MAPPING = std::array<T, 15>{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  return MAPPING[value];
}
template<CharType T>
[[nodiscard]] constexpr std::uint8_t isValidHex(const T ch) {
  if (ch >= static_cast<T>('0') && ch <= static_cast<T>('9')) {
    return true;
  }
  if (ch >= static_cast<T>('a') && ch <= static_cast<T>('f')) {
    return true;
  }
  if (ch >= static_cast<T>('A') && ch <= static_cast<T>('F')) {
    return true;
  }
  return false;
}

}

#endif//PF_COMMON_HEX_UTILS_H
