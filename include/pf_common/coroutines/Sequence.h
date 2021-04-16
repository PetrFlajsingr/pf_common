//
// Created by petr on 9/24/20.
//

#ifndef PF_COMMON_SEQUENCE_H
#define PF_COMMON_SEQUENCE_H

#include <cppcoro/generator.hpp>
#include <pf_common/concepts/Incrementable.h>

namespace pf {
/**
 * Iota implementation as an infinite coroutine.
 * @tparam T type to be returned
 * @param start starting value
 * @return generator for iota
 */
template<Incrementable T>
cppcoro::generator<T> iota(T start = T(0)) {
  while (true) {
    co_yield start;
    ++start;
  }
}

/**
 * Infinite coroutine, which repeats a given sequence.
 * @tparam T type to be returned
 * @param start starting value
 * @param mod modulo value, never gets reached
 * @return generator for mod repeat
 */
template<typename T>
requires ModCapable<T> && Incrementable<T> cppcoro::generator<T> modRepeat(T start, T mod) {
  start %= mod;
  while (true) {
    co_yield start;
    start = (start + 1) % mod;
  }
}

/**
 * Infinite coroutine, which repeats a given sequence.
 * @tparam T type to be returned
 * @param mod modulo value, never gets reached
 * @return generator for mod repeat
 */
template<typename T>
requires ModCapable<T> && Incrementable<T> cppcoro::generator<T> modRepeat(T mod) { return modRepeat(T{0}, mod); }

/**
 * Iterable range delimited by user.
 * @tparam T type to be returned
 * @param start starting value
 * @param end next to last value
 * @param step step for iterations
 * @return generator for range
 */
template<Incrementable T>
cppcoro::generator<T> range(T start, T end, T step = T(1)) {
  for (Incrementable auto i = start; i < end; i += step) { co_yield i; }
}
/**
 * Iterable range delimited by user.
 * @tparam T type to be returned
 * @param end next to last value
 * @return generator for range
 */
template<Incrementable T>
cppcoro::generator<T> range(T end) {
  return range(T(0), end);
}

/**
 * Utility function to access next value of a generator without using iterators.
 * @tparam T type returned by generator
 * @param generator generator to move
 * @return value returned by generator
 */
template<typename T>
T getNext(cppcoro::generator<T> &generator) {
  auto iter = generator.begin();
  return *iter;
}

}// namespace pf
#endif//PF_COMMON_SEQUENCE_H
