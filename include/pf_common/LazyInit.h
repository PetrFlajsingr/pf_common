//
// Created by Petr on 15.09.2020.
//

#ifndef PF_COMMON_LAZY_INIT_H
#define PF_COMMON_LAZY_INIT_H

#include <concepts>
#include <functional>
#include <memory>

namespace pf {

/**
 * @brief Lazy initialisation.
 *
 * Inner value is initialised on first access to it.
 * @tparam T inner value type
 */
template<typename T>
class LazyInit {
 public:
  using calc_fnc = std::function<T()>;
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;

  /**
   * Construct LazyInit.
   * @param calc function calculation the inner value
   */
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
    if (value == nullptr) { value = std::make_unique<T>(calc()); }
  }
  calc_fnc calc;
  std::unique_ptr<T> value;
};
}// namespace pf
#endif//PF_COMMON_LAZY_INIT_H
