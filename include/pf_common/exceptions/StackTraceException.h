/**
 * @file StackTraceException.h
 * @brief Exceptions with built in stacktrace dumping.
 * Macro STACKTRACE_ENABLE controls if the stacktrace reporting is enabled.
 * @author Petr Flajšingr
 * @date 23.9.20
 */
#ifndef PF_COMMON_STACKTRACEEXCEPTION_H
#define PF_COMMON_STACKTRACEEXCEPTION_H

#include <exception>
#include <fmt/format.h>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#ifdef STACKTRACE_ENABLE
#undef BACKWARD_HAS_BFD
#define BACKWARD_HAS_BFD 1
#include <backward.hpp>
#endif
#include <range/v3/view/enumerate.hpp>

namespace pf {
struct TraceData {
  std::string file;
  std::string function;
  uint32_t lineN;
  inline TraceData(std::string filename, std::string fncName, uint32_t lineNumber)
      : file(std::move(filename)), function(std::move(fncName)), lineN(lineNumber) {}
};
#ifdef STACKTRACE_ENABLE
inline std::vector<TraceData> getTrace(std::size_t skipN = 0) {
  auto result = std::vector<TraceData>{};
  auto stackTrace = backward::StackTrace{};
  auto resolver = backward::TraceResolver{};
  stackTrace.load_here();
  resolver.load_stacktrace(stackTrace);
  result.reserve(stackTrace.size());
  for (std::size_t i = skipN; i < stackTrace.size(); ++i) {
    const auto trace = resolver.resolve(stackTrace[i]);
    result.emplace_back(trace.source.filename, trace.object_function, trace.source.line);
  }

  return result;
}
#endif

inline std::string traceToString(const std::vector<TraceData> &traceData) {
  using namespace std::string_literals;
  auto result = ""s;
  for (const auto &[idx, trace] : ranges::views::enumerate(traceData)) {
    result += fmt::format("{} {} ({}:{})\n", idx, trace.function, trace.file, trace.lineN);
  }
  return result;
}

/**
 * @brief An exception providing stacktrace info.
 */
class StackTraceException : public std::exception {
 public:
  explicit StackTraceException(std::string_view fmt, auto &&...args) {
    using namespace std::string_view_literals;

    auto ss = std::stringstream{};
    if (!fmt.empty()) { ss << fmt::format("An exception occurred: {}\n", fmt::format(fmt, args...)); }
#ifdef STACKTRACE_ENABLE
    constexpr auto STACKTRACE_SKIP_N = 4;
    const auto traces = getTrace(STACKTRACE_SKIP_N);
    const auto CAUSED_BY = "Caused by:\n"sv;
    const auto padding = std::string(CAUSED_BY.size(), ' ');
    ss << CAUSED_BY;
    for (const auto &[idx, trace] : ranges::views::enumerate(traces)) {
      ss << fmt::format("{}#{} {} ({}:{})\n", padding, idx, trace.function, trace.file, trace.lineN);
    }
#endif
    whatStacktrace = ss.str();
  }

  [[nodiscard]] inline const char *what() const noexcept override { return whatStacktrace.c_str(); }

 private:
  std::string whatStacktrace;
};

/**
 * @brief Exception thrown when an invalid argument was provided.
 */
class InvalidArgumentException : public StackTraceException {
 public:
  explicit InvalidArgumentException(std::string_view fmt, auto &&...args) : StackTraceException(fmt, std::forward<decltype(args)>(args)...) {}
};
/**
 * @brief Exception thrown for not yet implemented functions etc.
 */
class NotImplementedException : public StackTraceException {
 public:
  explicit NotImplementedException(std::string_view fmt, auto &&...args) : StackTraceException(fmt, std::forward<decltype(args)>(args)...) {}
};

}// namespace pf
#endif//PF_COMMON_STACKTRACEEXCEPTION_H
