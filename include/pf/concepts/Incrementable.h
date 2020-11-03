//
// Created by petr on 9/24/20.
//

#ifndef VOXEL_RENDER_INCREMENTABLE_H
#define VOXEL_RENDER_INCREMENTABLE_H

#include <concepts>

namespace pf {
template<typename T>
concept Incrementable = requires(T t) {
  {++t};
  {t += t};
  { t + t }
  ->std::convertible_to<T>;
};

template<typename T>
concept ModCapable = requires(T t) {
  { t % t }
  ->std::convertible_to<T>;
};
}// namespace pf
#endif//VOXEL_RENDER_INCREMENTABLE_H
