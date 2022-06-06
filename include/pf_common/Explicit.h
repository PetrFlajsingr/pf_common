//
// Created by Petr on 6/6/2022.
//

#ifndef PF_COMMON_EXPLICIT_H
#define PF_COMMON_EXPLICIT_H

#include <concepts>
#include <utility>

namespace pf {

template<typename T>
  requires(std::movable<T> || std::copyable<T>)
struct Explicit {
  template<typename... Args>
    requires(std::constructible_from<T, Args...>)
  Explicit(Args &&...args) : value(std::forward<Args>(args)...) {
    static_assert(sizeof...(Args) != 0, "Provide explicit value for this argument");
  }

  operator T &&()
    requires(std::movable<T>)
  {
    return std::move(value);
  }
  operator T()
    requires(!std::movable<T>)
  {
    return value;
  }

  T value;
};
}// namespace pf

#endif//PF_COMMON_EXPLICIT_H
