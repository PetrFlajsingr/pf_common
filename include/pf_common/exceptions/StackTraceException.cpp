//
// Created by petr on 9/23/20.
//

#include "StackTraceException.h"
#include <utility>
#undef BACKWARD_HAS_BFD
#define BACKWARD_HAS_BFD 1
#include "backward.hpp"
#include <range/v3/view/enumerate.hpp>

using namespace backward;
using namespace std::string_literals;
using namespace std::string_view_literals;

StackTraceException::StackTraceException(std::string_view message) {
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

const char *StackTraceException::what() const noexcept { return whatStacktrace.c_str(); }

std::vector<TraceData> getTrace(std::size_t skipN) {
  auto result = std::vector<TraceData>{};
  auto stackTrace = StackTrace{};
  auto resolver = TraceResolver{};
  stackTrace.load_here();
  resolver.load_stacktrace(stackTrace);
  result.reserve(stackTrace.size());
  for (std::size_t i = skipN; i < stackTrace.size(); ++i) {
    const ResolvedTrace trace = resolver.resolve(stackTrace[i]);
    result.emplace_back(trace.source.filename, trace.object_function, trace.source.line);
  }

  return result;
}
std::string traceToString(const std::vector<TraceData> &traceData) {
  auto result = ""s;
  for (const auto &[idx, trace] : ranges::views::enumerate(traceData)) {
    result += fmt::format("{} {} ({}:{})\n", idx, trace.function, trace.file, trace.lineN);
  }
  return result;
}
TraceData::TraceData(std::string filename, std::string fncName, uint32_t lineNumber)
    : file(std::move(filename)), function(std::move(fncName)), lineN(lineNumber) {}

InvalidArgumentException::InvalidArgumentException(std::string_view message)
    : StackTraceException(message) {}

NotImplementedException::NotImplementedException(std::string_view message)
    : StackTraceException(message) {}
