/**
 * @file StringConvertible.h
 * @brief Provides concepts to check for to string conversion capabilities of a type along with a utility function for conversion.
 * @author Petr Flajšingr
 * @date 25.1.21
 */
#ifndef PF_COMMON_CONCEPTS_STRINGCONVERTIBLE_H
#define PF_COMMON_CONCEPTS_STRINGCONVERTIBLE_H

#include "StreamConcepts.h"
#include <sstream>

namespace pf {
/**
 * @brief A type which supports std::to_string
 * @tparam T
 */
template<typename T>
concept HasStdToString = requires(const T t) {
  { std::to_string(t) } -> std::convertible_to<std::string>;
};
/**
 * @brief A type which has T::toString() method.
 * @tparam T
 */
template<typename T>
concept HasToString = requires(const T t) {
  { t.toString() } -> std::convertible_to<std::string>;
};
/**
 * @brief A type which provides functionality to be converted into a string.
 * @tparam T
 */
template<typename T>
concept ToStringConvertible = StreamInputable<T> || HasStdToString<T> || HasToString<T>;

/**
 * Conversion of an object to string.
 * @tparam T type of the converted object
 * @param val value to be converted
 * @return string representation of the provided value
 */
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

#endif//PF_COMMON_CONCEPTS_STRINGCONVERTIBLE_H
