//
// Created by petr on 10/12/20.
//

#ifndef PF_COMMON_PTRCONSTRUCTIBLE_H
#define PF_COMMON_PTRCONSTRUCTIBLE_H

#include <memory>
namespace pf {
/**
 * @brief An implementation of static member functions providing interface for creating smart pointers
 * @tparam T
 * @todo: support for observable_ptr?
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
};
}// namespace pf
#endif//PF_COMMON_PTRCONSTRUCTIBLE_H
