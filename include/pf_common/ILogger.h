//
// Created by petr on 11/3/20.
//

#ifndef PF_COMMON_INCLUDE_PF_COMMON_ILOGGER_H
#define PF_COMMON_INCLUDE_PF_COMMON_ILOGGER_H

#include <string_view>
#include <utility>
#include <fmt/format.h>

namespace pf {
enum class LogLevel {
  Trace,
  Debug,
  Info,
  Warn,
  Err,
  Critical,
  Off
};

class ILogger {
 public:
  template<typename... Args>
  void logFmt(LogLevel level, std::string_view format, Args &&... args) {
    log(level, fmt::format(format, std::forward<Args>(args)...));
  }
  virtual void log(LogLevel level, std::string_view msg) = 0;
  virtual ~ILogger() = default;
};

class EmptyLogger : public ILogger {
 public:
  inline void log(LogLevel, std::string_vew) override {}
};

}// namespace pf
#endif//PF_COMMON_INCLUDE_PF_COMMON_ILOGGER_H
