/**
 * @file Exception.h
 * @author Petr Flajšingr
 * @date 23.9.20
 */
#ifndef PF_COMMON_EXCEPTIONS_H
#define PF_COMMON_EXCEPTIONS_H

#include <exception>
#include <fmt/format.h>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace pf {

/**
 * @brief An exception.
 */
class Exception : public std::exception {
 public:
  explicit Exception(std::string_view fmt, auto &&...args) {
    using namespace std::string_view_literals;

    auto ss = std::stringstream{};
    if (!fmt.empty()) { ss << fmt::format("An exception occurred: {}\n", fmt::format(fmt, args...)); }
    whatStr = ss.str();
  }

  [[nodiscard]] inline const char *what() const noexcept override { return whatStr.c_str(); }

 private:
  std::string whatStr;
};

/**
 * @brief Exception thrown when an invalid argument was provided.
 */
class InvalidArgumentException : public Exception {
 public:
  explicit InvalidArgumentException(std::string_view fmt, auto &&...args)
      : Exception(fmt, std::forward<decltype(args)>(args)...) {}
};
/**
 * @brief Exception thrown for not yet implemented functions etc.
 */
class NotImplementedException : public Exception {
 public:
  explicit NotImplementedException(std::string_view fmt, auto &&...args)
      : Exception(fmt, std::forward<decltype(args)>(args)...) {}
};

}// namespace pf
#endif//PF_COMMON_EXCEPTIONS_H
