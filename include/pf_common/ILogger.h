//
// Created by petr on 11/3/20.
//

#ifndef PF_COMMON_INCLUDE_PF_COMMON_ILOGGER_H
#define PF_COMMON_INCLUDE_PF_COMMON_ILOGGER_H

#include <fmt/format.h>
#include <string_view>
#include <utility>

namespace pf {
enum class LogLevel { Trace, Debug, Info, Warn, Err, Critical, Off };

class ILogger {
 public:
  inline explicit ILogger(std::string loggerName) : name(std::move(loggerName)) {}
  template<typename... Args>
  void logFmt(LogLevel level, std::string_view tag, std::string_view format, Args &&...args) {
    log(level, tag, fmt::format(format, std::forward<Args>(args)...));
  }
  [[nodiscard]] virtual std::string_view getName() const { return name; }
  virtual void log(LogLevel level, std::string_view tag, std::string_view msg) = 0;
  virtual ~ILogger() = default;

 private:
  std::string name;
};

class EmptyLogger : public ILogger {
 public:
  inline EmptyLogger() : ILogger("") {}
  inline void log(LogLevel, std::string_view, std::string_view) override {}
};

}// namespace pf
#endif//PF_COMMON_INCLUDE_PF_COMMON_ILOGGER_H
