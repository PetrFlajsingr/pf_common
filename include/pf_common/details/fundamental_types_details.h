//
// Created by xflajs00 on 04.02.2023.
//

#ifndef PF_COMMON_FUNDAMENTAL_TYPES_DETAILS_H
#define PF_COMMON_FUNDAMENTAL_TYPES_DETAILS_H

#include <cstdint>

namespace pf::details {

template<char... Digits>
consteval unsigned long long decToULL() {
  auto mul = 1ull;
  ((Digits, mul *= 10), ...);
  return (... + (mul /= 10, static_cast<unsigned long long>(Digits - '0') * mul));
}

template<char... Digits>
consteval unsigned long long binToULL() {
  auto mul = 1ull;
  ((Digits, mul *= 2), ...);
  return (... + (mul /= 2, static_cast<unsigned long long>(Digits - '0') * mul));
}
consteval char hexDigitToValue(char ch) {
  if (ch >= '0' && ch <= '9') {
    return static_cast<char>(ch - '0');
  } else if (ch >= 'a' && ch <= 'f') {
    return static_cast<char>(ch - 'a' + 10);
  } else {
    return static_cast<char>(ch - 'A' + 10);
  }
}
template<char... Digits>
consteval unsigned long long hexToULL() {
  auto mul = 1ull;
  ((Digits, mul *= 16ull), ...);
  return (... + (mul /= 16, static_cast<unsigned long long>(hexDigitToValue(Digits)) * mul));
}

template<char... Chars>
struct UDLCharsToULLConverter {
  constexpr static unsigned long long GetValue() { return decToULL<Chars...>(); }
};
template<char... Chars>
struct UDLCharsToULLConverter<'0', 'b', Chars...> {
  constexpr static unsigned long long GetValue() { return binToULL<Chars...>(); }
};
template<char... Chars>
struct UDLCharsToULLConverter<'0', 'B', Chars...> {
  constexpr static unsigned long long GetValue() { return binToULL<Chars...>(); }
};
template<char... Chars>
struct UDLCharsToULLConverter<'0', 'x', Chars...> {
  constexpr static unsigned long long GetValue() { return hexToULL<Chars...>(); }
};
template<char... Chars>
struct UDLCharsToULLConverter<'0', 'X', Chars...> {
  constexpr static unsigned long long GetValue() { return hexToULL<Chars...>(); }
};
}// namespace pf::details

#endif//PF_COMMON_FUNDAMENTAL_TYPES_DETAILS_H
