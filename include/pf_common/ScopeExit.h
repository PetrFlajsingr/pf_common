/**
 * @file ScopeExit.h
 * @brief Utility to emulate try finally construct.
 * @author Petr Flajšingr
 * @date 1.11.20
 */
#ifndef PF_COMMON_SCOPEEXIT_H
#define PF_COMMON_SCOPEEXIT_H

#include <concepts>
#include <functional>
#include <pf_common/macros.h>
#include <utility>

namespace pf {
/**
 * @brief Equivalent to try finally.
 */
template<std::invocable F>
class ScopeExit {
 public:
  /**
   * Construct RAII.
   * @param callable function to be called upon object's destruction
   */
  explicit ScopeExit(F &&callable) : callable(std::forward<F>(callable)) {}
  ScopeExit(ScopeExit &&other) noexcept : callable(std::move(other.callable)) { other.active = false; }
  ~ScopeExit() { std::invoke(callable); }

  ScopeExit &operator=(const ScopeExit &) const = delete;
  ScopeExit &operator=(ScopeExit &&other) noexcept {
    if (this == &other) { return *this; }
    callable = std::move(other.callable);
    other.active = false;
    return *this;
  }

 private:
  PF_NOUNIQUEADDRESS F callable;
  bool active = true;
};

template<typename F>
ScopeExit(F) -> ScopeExit<F>;

}// namespace pf

#endif//PF_COMMON_SCOPEEXIT_H
