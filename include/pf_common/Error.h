//
// Created by xflajs00 on 04.02.2023.
//

#ifndef PF_COMMON_ERROR_H
#define PF_COMMON_ERROR_H

#include <fmt/core.h>
#include <magic_enum.hpp>
#include <pf_common/concepts/Enum.h>
#include <string>

namespace pf {

template<Enum ErrorType, typename Payload = std::string>
struct Error {
  ErrorType type;
  Payload payload;

  [[nodiscard]] std::string toString() const { return fmt::format("{}: '{}'", magic_enum::enum_name(type), to_string(payload)); }
};

template<Enum ErrorType, typename Payload>
std::ostream &operator<<(std::ostream &o, const Error<ErrorType, Payload> &err) {
  o << err.toString();
  return o;
}
template<Enum ErrorType, typename Payload>
[[nodiscard]] std::string to_string(const Error<ErrorType, Payload> &err) {
  return err.toString();
}

}// namespace pf

#endif//PF_COMMON_ERROR_H
