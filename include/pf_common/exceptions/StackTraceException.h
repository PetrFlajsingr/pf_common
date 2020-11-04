//
// Created by petr on 9/23/20.
//

#ifndef PF_COMMON_STACKTRACEEXCEPTION_H
#define PF_COMMON_STACKTRACEEXCEPTION_H

#include <exception>
#include <fmt/format.h>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#undef BACKWARD_HAS_BFD
#define BACKWARD_HAS_BFD 1
#include <backward.hpp>
#include <range/v3/view/enumerate.hpp>

namespace pf {
struct TraceData {
  std::string file;
  std::string function;
  uint32_t lineN;
  inline TraceData(std::string filename, std::string fncName, uint32_t lineNumber)
      : file(std::move(filename)), function(std::move(fncName)), lineN(lineNumber) {}
};

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

inline std::string traceToString(const std::vector<TraceData> &traceData) {
  using namespace std::string_literals;
  auto result = ""s;
  for (const auto &[idx, trace] : ranges::views::enumerate(traceData)) {
    result += fmt::format("{} {} ({}:{})\n", idx, trace.function, trace.file, trace.lineN);
  }
  return result;
}

class StackTraceException : public std::exception {
 public:
  inline explicit StackTraceException(std::string_view message) {
    using namespace std::string_view_literals;
    constexpr auto STACKTRACE_SKIP_N = 4;
    const auto traces = getTrace(STACKTRACE_SKIP_N);
    auto ss = std::stringstream{};
    if (!message.empty()) { ss << fmt::format("An exception occurred: {}\n", message); }
    const auto CAUSED_BY = "Caused by:\n"sv;
    ss << CAUSED_BY;

    const auto padding = std::string(CAUSED_BY.size(), ' ');
    for (const auto &[idx, trace] : ranges::views::enumerate(traces)) {
      ss << fmt::format("{}#{} {} ({}:{})\n", padding, idx, trace.function, trace.file, trace.lineN);
    }
    whatStacktrace = ss.str();
  }
  static StackTraceException fmt(std::string_view fmt, auto &&... args) {
    return StackTraceException(fmt::format(fmt, args...));
  }

  [[nodiscard]] inline const char *what() const noexcept override {
    return whatStacktrace.c_str();
  }

 private:
  std::string whatStacktrace;
};

class InvalidArgumentException : public StackTraceException {
 public:
  inline explicit InvalidArgumentException(std::string_view message)
      : StackTraceException(message) {}
};

class NotImplementedException : public StackTraceException {
 public:
  inline explicit NotImplementedException(std::string_view message)
      : StackTraceException(message) {}
};

}// namespace pf
#endif//PF_COMMON_STACKTRACEEXCEPTION_H
