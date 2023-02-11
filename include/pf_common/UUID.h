/**
 * @file UUID.h
 * @brief UUID class.
 * @author Petr Flajšingr
 * @date 29.1.23
 */

#ifndef PF_COMMON_UUID_H
#define PF_COMMON_UUID_H

#include <pf_common/hex_utils.h>
#include <pf_common/algorithms.h>
#include <string>

#if !defined(PF_UUID_RUNTIME_VALIDITY_CHECK_ENABLED)
#define PF_UUID_RUNTIME_VALIDITY_CHECK_ENABLED false
#endif

#if !defined(PF_UUID_RUNTIME_ASSERT)
#include <cassert>
#define PF_UUID_RUNTIME_ASSERT(condition, message) assert((condition) && message)
#endif

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 5045)
#endif

namespace pf {
// UDLs
class UUID;
namespace uuid_literals {
[[nodiscard]] consteval UUID operator""_uuid(const char *str, std::size_t size);
[[nodiscard]] consteval UUID operator""_uuid(const wchar_t *str, std::size_t size);
[[nodiscard]] consteval UUID operator""_uuid(const char8_t *str, std::size_t size);
[[nodiscard]] consteval UUID operator""_uuid(const char16_t *str, std::size_t size);
[[nodiscard]] consteval UUID operator""_uuid(const char32_t *str, std::size_t size);
}// namespace uuid_literals
/**
 * Check if a string is in valid UUID formt.
 * @tparam T char type
 * @param uuid string to check
 * @return true if the string is valid UUID, false otherwise
 */
template<CharType T>
[[nodiscard]] constexpr bool isValidUUID(std::basic_string_view<T> uuid) {
  if (uuid.empty() || uuid.size() != 36) { return false; }
  for (std::size_t i = 0; i < 8; ++i) {
    if (!isValidHex(uuid[i])) { return false; }
  }
  if (uuid[8] != static_cast<T>('-')) { return false; }
  for (std::size_t i = 9; i < 13; ++i) {
    if (!isValidHex(uuid[i])) { return false; }
  }
  if (uuid[13] != static_cast<T>('-')) { return false; }
  for (std::size_t i = 14; i < 18; ++i) {
    if (!isValidHex(uuid[i])) { return false; }
  }
  if (uuid[18] != static_cast<T>('-')) { return false; }
  for (std::size_t i = 19; i < 23; ++i) {
    if (!isValidHex(uuid[i])) { return false; }
  }
  if (uuid[23] != static_cast<T>('-')) { return false; }
  for (std::size_t i = 24; i < 36; ++i) {
    if (!isValidHex(uuid[i])) { return false; }
  }
  return true;
}

/**
 * @brief Unique 128 bit identifier.
 *
 * @note data.data() is used inside the class, because otherwise it won't work constexpr in debug builds
 */
class UUID {
 public:
  constexpr UUID() = default;
  template<CharType T>
  consteval explicit UUID(const T uuid[36]) : UUID{std::basic_string_view<T>{uuid, 36}} {}

  template<CharType T>
  constexpr explicit UUID(std::basic_string_view<T> uuid) {
    if (uuid.empty()) { return; }
    if (std::is_constant_evaluated()) {
      if (!isValidUUID(uuid)) { throw "Invalid UUID provided"; }// FIXME: this needs better reporting, would work with if constval better
    } else if constexpr (PF_UUID_RUNTIME_VALIDITY_CHECK_ENABLED) {
      PF_UUID_RUNTIME_ASSERT(isValidUUID(uuid), "Invalid UUID");
    }

    if (uuid.size() != 36) { return; }

    std::size_t cnt{0};
    for (std::size_t i = 0; i < 8; i += 2) {
      const auto tmp = hexDigitToValue(uuid[i]);
      data.data()[cnt++] = static_cast<std::byte>(tmp * 16 + hexDigitToValue(uuid[i + 1]));
    }
    for (std::size_t i = 9; i < 13; i += 2) {
      const auto tmp = hexDigitToValue(uuid[i]);
      data.data()[cnt++] = static_cast<std::byte>(tmp * 16 + hexDigitToValue(uuid[i + 1]));
    }
    for (std::size_t i = 14; i < 18; i += 2) {
      const auto tmp = hexDigitToValue(uuid[i]);
      data.data()[cnt++] = static_cast<std::byte>(tmp * 16 + hexDigitToValue(uuid[i + 1]));
    }
    for (std::size_t i = 19; i < 23; i += 2) {
      const auto tmp = hexDigitToValue(uuid[i]);
      data.data()[cnt++] = static_cast<std::byte>(tmp * 16 + hexDigitToValue(uuid[i + 1]));
    }
    for (std::size_t i = 24; i < 36; i += 2) {
      const auto tmp = hexDigitToValue(uuid[i]);
      data.data()[cnt++] = static_cast<std::byte>(tmp * 16 + hexDigitToValue(uuid[i + 1]));
    }
  }
  constexpr explicit UUID(std::array<std::byte, 16> initData) : data{initData} {}
  constexpr UUID(const UUID &other) = default;
  constexpr UUID &operator=(const UUID &other) = default;

  template<typename CharT = char, typename Traits = std::char_traits<CharT>, typename Allocator = std::allocator<CharT>>
  [[nodiscard]] friend std::basic_string<CharT, Traits, Allocator> to_string(const UUID &uuid) {
    std::basic_string<CharT, Traits, Allocator> result{NilUUIDString<CharT>};
    constexpr static auto HYPHEN_INDICES = std::array<std::size_t, 4>{8, 13, 18, 23};
    for (std::size_t i = 0, index = 0; i < 36; ++i) {
      if (contains(HYPHEN_INDICES, i)) { continue; }
      result[i] = valueToHexDigit<CharT>(static_cast<std::uint8_t>(uuid.data.data()[index]) >> 4 & 0x0F);
      result[i + 1] = valueToHexDigit<CharT>(static_cast<std::uint8_t>(uuid.data.data()[index]) & 0x0F);
      ++index;
      ++i;
    }
    return result;
  }
  [[nodiscard]] friend constexpr bool operator==(const UUID &lhs, const UUID &rhs) { return lhs.data == rhs.data; }
  template<class CharT, class Traits>
  friend std::basic_ostream<CharT, Traits> &operator<<(std::basic_ostream<CharT, Traits> &s, const UUID &id) {
    s << to_string(id);
    return s;
  }

  [[nodiscard]] constexpr const std::array<std::byte, 16> &getData() const { return data; }

 private:
  template<typename CharT>
  constexpr static CharT NilUUIDString[37] = "00000000-0000-0000-0000-000000000000";
  std::array<std::byte, 16> data{{std::byte{0}}};
};
namespace uuid_literals {
[[nodiscard]] consteval UUID operator""_uuid(const char *str, std::size_t size) { return UUID{std::basic_string_view{str, size}}; }
[[nodiscard]] consteval UUID operator""_uuid(const wchar_t *str, std::size_t size) { return UUID{std::basic_string_view{str, size}}; }
[[nodiscard]] consteval UUID operator""_uuid(const char8_t *str, std::size_t size) { return UUID{std::basic_string_view{str, size}}; }
[[nodiscard]] consteval UUID operator""_uuid(const char16_t *str, std::size_t size) { return UUID{std::basic_string_view{str, size}}; }
[[nodiscard]] consteval UUID operator""_uuid(const char32_t *str, std::size_t size) { return UUID{std::basic_string_view{str, size}}; }
}// namespace uuid_literals

}// namespace pf

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif//PF_COMMON_UUID_H
