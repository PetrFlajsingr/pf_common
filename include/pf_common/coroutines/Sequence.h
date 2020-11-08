//
// Created by petr on 9/24/20.
//

#ifndef PF_COMMON_SEQUENCE_H
#define PF_COMMON_SEQUENCE_H

#include <cppcoro/generator.hpp>
#include <pf_common/concepts/Incrementable.h>

namespace pf {
template<Incrementable T>
cppcoro::generator<T> iota(T start = T(0)) {
  while (true) {
    co_yield start;
    ++start;
  }
}

template<typename T>
requires ModCapable<T> &&Incrementable<T> cppcoro::generator<T> modRepeat(T start, T mod) {
  while (true) {
    co_yield start;
    start = (start + 1) % mod;
  }
}

template<typename T>
requires ModCapable<T> &&Incrementable<T> cppcoro::generator<T> modRepeat(T mod) {
  return modRepeat(T{0}, mod);
}

template<Incrementable T>
cppcoro::generator<T> range(T start, T end, T step = T(1)) {
  for (Incrementable auto i = start; i < end; i += step) { co_yield i; }
}
template<Incrementable T>
cppcoro::generator<T> range(T end) {
  return range(T(0), end);
}

template<typename T>
T getNext(cppcoro::generator<T> &generator) {
  auto iter = generator.begin();
  return *iter;
}

}// namespace pf
#endif//PF_COMMON_SEQUENCE_H
