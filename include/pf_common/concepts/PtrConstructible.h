//
// Created by petr on 10/12/20.
//

#ifndef VOXEL_RENDER_PTRCONSTRUCTIBLE_H
#define VOXEL_RENDER_PTRCONSTRUCTIBLE_H

#include <memory>
namespace pf {
template<typename T>
struct PtrConstructible {
  template<typename... Args>
  static std::shared_ptr<T>
  CreateShared(Args &&... args) requires std::constructible_from<T, Args...> {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }
  template<typename... Args>
  static std::unique_ptr<T>
  CreateUnique(Args &&... args) requires std::constructible_from<T, Args...> {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }
};
}// namespace pf_common
#endif//VOXEL_RENDER_PTRCONSTRUCTIBLE_H
