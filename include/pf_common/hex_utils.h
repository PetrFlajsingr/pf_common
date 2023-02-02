//
// Created by xflajs00 on 29.01.2023.
//

#ifndef PF_COMMON_HEX_UTILS_H
#define PF_COMMON_HEX_UTILS_H

#include <array>
#include <pf_common/array.h>
#include <pf_common/concepts/FundamentalTypes.h>

namespace pf {

template<CharType T>
[[nodiscard]] constexpr std::uint8_t hexDigitToValue(const T ch) {
  if (ch >= static_cast<T>('0') && ch <= static_cast<T>('9')) { return static_cast<std::uint8_t>(ch - static_cast<T>('0')); }
  if (ch >= static_cast<T>('a') && ch <= static_cast<T>('f')) { return static_cast<std::uint8_t>(10 + ch - static_cast<T>('a')); }
  if (ch >= static_cast<T>('A') && ch <= static_cast<T>('F')) { return static_cast<std::uint8_t>(10 + ch - static_cast<T>('A')); }
  return 0;
}
template<CharType T>
[[nodiscard]] constexpr T valueToHexDigit(const std::uint8_t value) {
  //FIXME: check input validity - contracts?
  constexpr auto MAPPING = make_array<T>(T{'0'}, T{'1'}, T{'2'}, T{'3'}, T{'4'}, T{'5'}, T{'6'}, T{'7'}, T{'8'}, T{'9'}, T{'a'}, T{'b'},
                                         T{'c'}, T{'d'}, T{'e'}, T{'f'});
  return MAPPING[value];
}
template<CharType T>
[[nodiscard]] constexpr std::uint8_t isValidHex(const T ch) {
  if (ch >= static_cast<T>('0') && ch <= static_cast<T>('9')) { return true; }
  if (ch >= static_cast<T>('a') && ch <= static_cast<T>('f')) { return true; }
  if (ch >= static_cast<T>('A') && ch <= static_cast<T>('F')) { return true; }
  return false;
}

}// namespace pf

#endif//PF_COMMON_HEX_UTILS_H
