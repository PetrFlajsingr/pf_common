//
// Created by petr on 11/1/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UTILS_RAII_H
#define REALISTIC_VOXEL_RENDERING_UTILS_RAII_H

#include <concepts>
#include <functional>
#include <utility>

namespace pf {
class RAII {
 public:
  inline explicit RAII(std::invocable auto &&callable)
      : callable(std::forward<decltype(callable)>(callable)) {}

  inline ~RAII() { std::invoke(callable); }

 private:
  std::function<void()> callable;
};
}// namespace pf_common

#endif//REALISTIC_VOXEL_RENDERING_UTILS_RAII_H
