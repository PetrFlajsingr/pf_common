//
// Created by Petr on 18.11.2019.
//

#ifndef PF_COMMON_BYTELITERALS_H
#define PF_COMMON_BYTELITERALS_H

namespace pf {
namespace detail {
constexpr unsigned long long byteOrderMultiplier = 1000;
constexpr unsigned long long biByteOrderMultiplier = 1024;

inline constexpr unsigned long long pow(unsigned long long a, unsigned long long b) {
  auto result = a;
  for (std::size_t i = 1; i < b; ++i) {
    result *= a;
  }
  return result;
}
}// namespace detail
constexpr unsigned long long operator"" _B(unsigned long long value) { return value; }
constexpr unsigned long long operator"" _KB(unsigned long long value) { return value * detail::byteOrderMultiplier; }
constexpr unsigned long long operator"" _MB(unsigned long long value) {
  return value * detail::pow(detail::byteOrderMultiplier, 2);
}
constexpr unsigned long long operator"" _GB(unsigned long long value) {
  return value * detail::pow(detail::byteOrderMultiplier, 3);
}
constexpr unsigned long long operator"" _TB(unsigned long long value) {
  return value * detail::pow(detail::byteOrderMultiplier, 4);
}
constexpr unsigned long long operator"" _PB(unsigned long long value) {
  return value * detail::pow(detail::byteOrderMultiplier, 5);
}

constexpr unsigned long long operator"" _KiB(unsigned long long value) { return value * detail::biByteOrderMultiplier; }
constexpr unsigned long long operator"" _MiB(unsigned long long value) {
  return value * detail::pow(detail::biByteOrderMultiplier, 2);
}
constexpr unsigned long long operator"" _GiB(unsigned long long value) {
  return value * detail::pow(detail::biByteOrderMultiplier, 3);
}
constexpr unsigned long long operator"" _TiB(unsigned long long value) {
  return value * detail::pow(detail::biByteOrderMultiplier, 4);
}
constexpr unsigned long long operator"" _PiB(unsigned long long value) {
  return value * detail::pow(detail::biByteOrderMultiplier, 5);
}

}// namespace pf

#endif// PF_COMMON_BYTELITERALS_H
