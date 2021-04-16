//
// Created by petr on 11/3/20.
//

#ifndef PF_COMMON_INCLUDE_PF_COMMON_ILOGGER_H
#define PF_COMMON_INCLUDE_PF_COMMON_ILOGGER_H

#include <fmt/format.h>
#include <string_view>
#include <utility>

namespace pf {
/**
 * Logging levels.
 */
enum class LogLevel { Trace, Debug, Info, Warn, Err, Critical, Off };

/**
 * @brief Common interface for logging.
 * @todo: merge log and logFmt
 */
class ILogger {
 public:
  /**
   * Create ILogger.
   * @param loggerName name of the logger
   */
  inline explicit ILogger(std::string loggerName) : name(std::move(loggerName)) {}
  /**
   * Log data and format them.
   * @tparam Args arg types for formatting
   * @param level log level
   * @param tag message tag
   * @param format format string
   * @param args arguments for formatting
   */
  template<typename... Args>
  void logFmt(LogLevel level, std::string_view tag, std::string_view format, Args &&...args) {
    log(level, tag, fmt::format(format, std::forward<Args>(args)...));
  }
  /**
   * Get logger name.
   * @return logger name
   */
  [[nodiscard]] virtual std::string_view getName() const { return name; }
  /**
   * Log data.
   * @param level log level
   * @param tag message tag
   * @param msg log message
   */
  virtual void log(LogLevel level, std::string_view tag, std::string_view msg) = 0;
  virtual ~ILogger() = default;

 private:
  std::string name;
};

/**
 * @brief ILogger implementation which does nothing.
 */
class EmptyLogger : public ILogger {
 public:
  /**
   * Construct EmptyLogger.
   */
  inline EmptyLogger() : ILogger("") {}
  inline void log(LogLevel, std::string_view, std::string_view) override {}
};

}// namespace pf
#endif//PF_COMMON_INCLUDE_PF_COMMON_ILOGGER_H
