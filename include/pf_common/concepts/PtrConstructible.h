/**
 * @file PtrConstructible.h
 * @brief Interface for alternative constructor-like static methods to create smart pointers.
 * @author Petr Flajšingr
 * @date 12.10.20
 */
#ifndef PF_COMMON_PTRCONSTRUCTIBLE_H
#define PF_COMMON_PTRCONSTRUCTIBLE_H

#include <experimental/memory>
#include <memory>
namespace pf {
/**
 * @brief An implementation of static member functions providing interface for creating smart pointers
 * @tparam T
 */
template<typename T>
struct PtrConstructible {
  template<typename... Args>
  static std::shared_ptr<T> CreateShared(Args &&...args) requires std::constructible_from<T, Args...> {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }
  template<typename... Args>
  static std::unique_ptr<T> CreateUnique(Args &&...args) requires std::constructible_from<T, Args...> {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }
  std::experimental::observer_ptr<T> createObserver() {
    return std::experimental::make_observer(this);
  }
};
}// namespace pf
#endif//PF_COMMON_PTRCONSTRUCTIBLE_H
