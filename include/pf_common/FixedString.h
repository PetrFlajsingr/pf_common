/**
 * @file FixedString.h
 * @brief Constexpr fixed string.
 * @author Petr Flajšingr
 * @date 7.2.22
 */

#ifndef PF_COMMON_FIXEDSTRING_H
#define PF_COMMON_FIXEDSTRING_H

#include <cstddef>

namespace pf {

/**
 * @brief Simple constexpr friendly fixed string.
 * @tparam N Size of the string including terminating \0
 */
template<std::size_t N = 1>
struct FixedString {
  constexpr FixedString() = default;
  constexpr FixedString(const char (&arr)[N]) {
    for (std::size_t i = 0; i < N; ++i) { str[i] = arr[i]; }
  }

  template<std::size_t OtherN>
  [[nodiscard]] constexpr bool operator==(const FixedString<OtherN> &other) const {
    if (N != OtherN) { return false; }

    for (auto i1 = begin(), i2 = other.begin(); i1 != end(); ++i1, ++i2) {
      if (*i1 != *i2) { return false; }
    }
    return true;
  }
  template<std::size_t OtherN>
  [[nodiscard]] constexpr bool operator==(const char (&arr)[OtherN]) const {
    if (N != OtherN) { return false; }

    for (auto i1 = begin(), i2 = arr; i1 != end(); ++i1, ++i2) {
      if (*i1 != *i2) { return false; }
    }
    return true;
  }

  template<std::size_t OtherN>
  [[nodiscard]] constexpr bool operator!=(const FixedString<OtherN> &other) const {
    return !(*this == other);
  }

  template<std::size_t OtherN>
  [[nodiscard]] constexpr bool operator!=(const char (&arr)[OtherN]) const {
    return !(*this == arr);
  }

  template<std::size_t N2>
  [[nodiscard]] constexpr FixedString<N + N2 - 1> concat(const FixedString<N2> &str2) const {
    auto result = FixedString<N + N2 - 1>{};
    for (std::size_t i = 0; i < N - 1; ++i) { result.str[i] = str[i]; }
    for (std::size_t i = 0; i < N2 - 1; ++i) { result.str[N - 1 + i] = str2.str[i]; }
    result.str[N + N2 - 2] = '\0';
    return result;
  }

  template<std::size_t N2>
  [[nodiscard]] constexpr FixedString<N + N2 - 1> operator+(const FixedString<N2> &str2) const {
    return concat(str2);
  }
  template<std::size_t N2>
  [[nodiscard]] constexpr FixedString<N + N2 - 1> concat(char const (&arr)[N2]) const {
    auto result = FixedString<N + N2 - 1>{};
    for (std::size_t i = 0; i < N - 1; ++i) { result.str[i] = str[i]; }
    for (std::size_t i = 0; i < N2 - 1; ++i) { result.str[N - 1 + i] = arr[i]; }
    result.str[N + N2 - 2] = '\0';
    return result;
  }
  template<std::size_t N2>
  [[nodiscard]] constexpr FixedString<N + N2 - 1> operator+(char const (&arr)[N2]) const {
    return concat(arr);
  }
  [[nodiscard]] constexpr std::size_t size() const { return N - 1; }
  [[nodiscard]] constexpr std::size_t length() const { return size(); }
  [[nodiscard]] constexpr const char *begin() const { return str; }
  [[nodiscard]] constexpr const char *end() const { return str + N; }
  [[nodiscard]] constexpr char operator[](std::size_t index) const { return str[index]; }
  [[nodiscard]] constexpr char front() const { return str[0]; }
  [[nodiscard]] constexpr char back() const { return str[N + 1]; }
  [[nodiscard]] constexpr const char *data() const { return str; }
  [[nodiscard]] constexpr const char *c_str() const { return str; }
  char str[N] = "\0";
};

namespace FixedStringLiteral {
template<FixedString str>
constexpr auto operator""_fixed_str() {
  return str;
}

}// namespace FixedStringLiteral

}// namespace pf

#endif//PF_COMMON_FIXEDSTRING_H
