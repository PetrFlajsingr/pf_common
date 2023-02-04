//
// Created by xflajs00 on 04.02.2023.
//

#ifndef PF_COMMON_FUNDAMENTAL_TYPES_H
#define PF_COMMON_FUNDAMENTAL_TYPES_H

#include <cstddef>
#include <cstdint>
#include <limits>
#include <pf_common/details/fundamental_types_details.h>
#include <type_traits>

#if !defined(PF_FUNDAMENTAL_TYPES_UDL_CHECK_ENABLED)
#define PF_FUNDAMENTAL_TYPES_UDL_CHECK_ENABLED true
#else
static_assert(std::same_as<decltype(PF_FUNDAMENTAL_TYPES_UDL_CHECK_ENABLED), bool>,
              "PF_FUNDAMENTAL_TYPES_UDL_CHECK_ENABLED has to be a bool");
#endif

namespace pf {

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using usize = std::size_t;
using isize = std::make_signed_t<usize>;

using byte = std::byte;

using f32 = float;
using f64 = double;

namespace fundamental_literals {
#if PF_FUNDAMENTAL_TYPES_UDL_CHECK_ENABLED
template<char... Chars>
[[nodiscard]] consteval i8 operator""_i8() {
  constexpr auto value = details::UDLCharsToULLConverter<Chars...>::GetValue();
  static_assert(value <= std::numeric_limits<i8>::max());
  return static_cast<i8>(value);
}
template<char... Chars>
[[nodiscard]] consteval i16 operator""_i16() {
  constexpr auto value = details::UDLCharsToULLConverter<Chars...>::GetValue();
  static_assert(value <= std::numeric_limits<i16>::max());
  return static_cast<i16>(value);
}
template<char... Chars>
[[nodiscard]] consteval i32 operator""_i32() {
  constexpr auto value = details::UDLCharsToULLConverter<Chars...>::GetValue();
  static_assert(value <= std::numeric_limits<i32>::max());
  return static_cast<i32>(value);
}
template<char... Chars>
[[nodiscard]] consteval i64 operator""_i64() {
  constexpr auto value = details::UDLCharsToULLConverter<Chars...>::GetValue();
  static_assert(value <= std::numeric_limits<i64>::max());
  return static_cast<i64>(value);
}

template<char... Chars>
[[nodiscard]] consteval u8 operator""_u8() {
  constexpr auto value = details::UDLCharsToULLConverter<Chars...>::GetValue();
  static_assert(value <= std::numeric_limits<u8>::max());
  return static_cast<u8>(value);
}
template<char... Chars>
[[nodiscard]] consteval u16 operator""_u16() {
  constexpr auto value = details::UDLCharsToULLConverter<Chars...>::GetValue();
  static_assert(value <= std::numeric_limits<u16>::max());
  return static_cast<u16>(value);
}
template<char... Chars>
[[nodiscard]] consteval u32 operator""_u32() {
  constexpr auto value = details::UDLCharsToULLConverter<Chars...>::GetValue();
  static_assert(value <= std::numeric_limits<u32>::max());
  return static_cast<u32>(value);
}
template<char... Chars>
[[nodiscard]] consteval u64 operator""_u64() {
  constexpr auto value = details::UDLCharsToULLConverter<Chars...>::GetValue();
  return static_cast<u64>(value);
}

template<char... Chars>
[[nodiscard]] consteval isize operator""_isize() {
  constexpr auto value = details::UDLCharsToULLConverter<Chars...>::GetValue();
  static_assert(value <= static_cast<unsigned long long>(std::numeric_limits<isize>::max()));
  return static_cast<isize>(value);
}
template<char... Chars>
[[nodiscard]] consteval usize operator""_usize() {
  constexpr auto value = details::UDLCharsToULLConverter<Chars...>::GetValue();
  return static_cast<usize>(value);
}

[[nodiscard]] consteval f32 operator""_f32(long double value) { return static_cast<f32>(value); }
[[nodiscard]] consteval f64 operator""_f64(long double value) { return static_cast<f64>(value); }
[[nodiscard]] consteval f32 operator""_f32(unsigned long long value) { return static_cast<f32>(value); }
[[nodiscard]] consteval f64 operator""_f64(unsigned long long value) { return static_cast<f64>(value); }

template<char... Chars>
[[nodiscard]] consteval byte operator""_byte() {
  constexpr auto value = details::UDLCharsToULLConverter<Chars...>::GetValue();
  static_assert(value <= std::numeric_limits<std::underlying_type_t<byte>>::max());
  return static_cast<byte>(value);
}
#else
[[nodiscard]] consteval i8 operator""_i8(unsigned long long value) { return static_cast<i8>(value); }
[[nodiscard]] consteval i16 operator""_i16(unsigned long long value) { return static_cast<i16>(value); }
[[nodiscard]] consteval i32 operator""_i32(unsigned long long value) { return static_cast<i32>(value); }
[[nodiscard]] consteval i64 operator""_i64(unsigned long long value) { return static_cast<i64>(value); }

[[nodiscard]] consteval u8 operator""_u8(unsigned long long value) { return static_cast<u8>(value); }
[[nodiscard]] consteval u16 operator""_u16(unsigned long long value) { return static_cast<u16>(value); }
[[nodiscard]] consteval u32 operator""_u32(unsigned long long value) { return static_cast<u32>(value); }
[[nodiscard]] consteval u64 operator""_u64(unsigned long long value) { return static_cast<u64>(value); }

[[nodiscard]] consteval isize operator""_isize(unsigned long long value) { return static_cast<isize>(value); }
[[nodiscard]] consteval usize operator""_usize(unsigned long long value) { return static_cast<usize>(value); }

[[nodiscard]] consteval f32 operator""_f32(long double value) { return static_cast<f32>(value); }
[[nodiscard]] consteval f64 operator""_f64(long double value) { return static_cast<f64>(value); }
[[nodiscard]] consteval f32 operator""_f32(unsigned long long value) { return static_cast<f32>(value); }
[[nodiscard]] consteval f64 operator""_f64(unsigned long long value) { return static_cast<f64>(value); }

[[nodiscard]] consteval byte operator""_byte(unsigned long long value) { return static_cast<byte>(value); }
#endif
}// namespace fundamental_literals
}// namespace pf

#endif//PF_COMMON_FUNDAMENTAL_TYPES_H
