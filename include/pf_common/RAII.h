/**
 * @file RAII.h
 * @brief Utility to emulate try finally construct.
 * @author Petr Flajšingr
 * @date 1.11.20
 */
#ifndef PF_COMMON_UTILS_RAII_H
#define PF_COMMON_UTILS_RAII_H

#include <concepts>
#include <functional>
#include <utility>

namespace pf {
/**
 * @brief Equivalent to try finally.
 */
class RAII {
 public:
  /**
   * Construct RAII.
   * @param callable function to be called upon object's destruction
   */
  explicit RAII(std::invocable auto &&callable) : callable(std::forward<decltype(callable)>(callable)) {}

  inline ~RAII() { std::invoke(callable); }

 private:
  std::function<void()> callable;
};
}// namespace pf

#endif//PF_COMMON_UTILS_RAII_H
