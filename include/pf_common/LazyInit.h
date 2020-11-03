//
// Created by Petr on 15.09.2020.
//

#ifndef DESIGN_PATTERNS_LAZY_INIT_H
#define DESIGN_PATTERNS_LAZY_INIT_H

#include "../concepts.h"
#include <functional>

namespace pf {
template<typename T>
class LazyInit {
 public:
  using calc_fnc = std::function<T()>;
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;

  explicit LazyInit(std::invocable auto calc) : calc(calc) {}

  pointer operator->() {
    calculate();
    return &*value;
  }

  reference operator*() {
    calculate();
    return *value;
  }

  const_pointer operator->() const {
    calculate();
    return *&value;
  }

  const_reference operator*() const {
    calculate();
    return *value;
  }

 private:
  void calculate() {
    if (value == nullptr) {
      value = std::make_unique<T>(calc());
    }
  }
  calc_fnc calc;
  std::unique_ptr<T> value;
};
}// namespace pf_common
#endif//DESIGN_PATTERNS_LAZY_INIT_H
