//
// Created by petr on 9/24/20.
//

#ifndef PF_COMMON_INCREMENTABLE_H
#define PF_COMMON_INCREMENTABLE_H

#include <concepts>

namespace pf {
template<typename T>
concept Incrementable = requires(T t) {
  {++t};
  {t += t};
  { t + t } -> std::convertible_to<T>;
};

template<typename T>
concept ModCapable = requires(T t) {
  { t % t } -> std::convertible_to<T>;
};
}// namespace pf
#endif//PF_COMMON_INCREMENTABLE_H
