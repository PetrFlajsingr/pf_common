//
// Created by petr on 1/4/21.
//

#ifndef REALISTIC_VOXEL_RENDERING_SRC_UTILS_BIN_H
#define REALISTIC_VOXEL_RENDERING_SRC_UTILS_BIN_H

#include "concepts/Serializable.h"

namespace pf {
template<typename T>
std::vector<std::byte> toBytes(const T &data) {
  if constexpr (Serializable<T>) { return data.serialize(); }
  auto result = std::vector<std::byte>(sizeof(data));
  std::ranges::copy(std::span(reinterpret_cast<const std::byte *>(&data), sizeof(data)), result.begin());
  return result;
}

template<typename T>
requires std::is_default_constructible_v<T> T fromBytes(std::span<const std::byte> data) {
  assert(data.size() == sizeof(T));
  auto result = T{};
  std::ranges::copy(data, reinterpret_cast<std::byte *>(&result));
  return result;
}

}// namespace pf

#endif//REALISTIC_VOXEL_RENDERING_SRC_UTILS_BIN_H
