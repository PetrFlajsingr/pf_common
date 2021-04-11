//
// Created by petr on 11/1/20.
//

#ifndef PF_COMMONING_UTILS_RAII_H
#define PF_COMMONING_UTILS_RAII_H

#include <concepts>
#include <functional>
#include <utility>

namespace pf {
class RAII {
 public:
  explicit RAII(std::invocable auto &&callable) : callable(std::forward<decltype(callable)>(callable)) {}

  inline ~RAII() { std::invoke(callable); }

 private:
  std::function<void()> callable;
};
}// namespace pf

#endif//PF_COMMONING_UTILS_RAII_H
