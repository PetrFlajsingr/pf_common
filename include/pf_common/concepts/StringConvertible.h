//
// Created by xflajs00 on 25.01.2021.
//

#ifndef PF_COMMON_INCLUDE_PF_COMMON_CONCEPTS_STRINGCONVERTIBLE_H
#define PF_COMMON_INCLUDE_PF_COMMON_CONCEPTS_STRINGCONVERTIBLE_H

#include "StreamConcepts.h"
#include <sstream>

namespace pf {

template<typename T>
concept HasStdToString = requires(const T t) {
  { std::to_string(t) } -> std::convertible_to<std::string>;
};
template<typename T>
concept HasToString = requires(const T t) {
  { t.toString() } -> std::convertible_to<std::string>;
};

template<typename T>
concept ToStringConvertible = StreamInputable<T> || HasStdToString<T> || HasToString<T>;

template<ToStringConvertible T>
std::string toString(const T &val) {
  if constexpr (StreamInputable<T>) {
    auto ss = std::stringstream();
    ss << val;
    return ss.str();
  } else if constexpr (HasStdToString<T>) {
    return std::to_string(val);
  } else if constexpr (HasToString<T>) {
    return val.toString();
  }
}
}// namespace pf

#endif//PF_COMMON_INCLUDE_PF_COMMON_CONCEPTS_STRINGCONVERTIBLE_H
