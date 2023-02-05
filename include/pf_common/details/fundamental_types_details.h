//
// Created by xflajs00 on 04.02.2023.
//

#ifndef PF_COMMON_FUNDAMENTAL_TYPES_DETAILS_H
#define PF_COMMON_FUNDAMENTAL_TYPES_DETAILS_H

#include <cstdint>

namespace pf::details {

template<unsigned long long Radix, char... Digits>
consteval unsigned long long digitsToULL() {
  auto mul = 1ull;
  auto result = 0ull;
  auto dummy = 0ull;
  ((result += static_cast<unsigned long long>(Digits) * mul, mul *= Radix, dummy) = ... = 0);
  return result;
}

template<char... Digits>
consteval unsigned long long decToULL() {
  return digitsToULL<10ull, Digits - '0' ...>();
}

template<char... Digits>
consteval unsigned long long binToULL() {
  return digitsToULL<2ull, Digits - '0' ...>();
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
  return digitsToULL<16ull, hexDigitToValue(Digits)...>();
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
