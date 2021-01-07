//
// Created by petr on 1/2/21.
//

#ifndef REALISTIC_VOXEL_RENDERING_SRC_UTILS_BITS_H
#define REALISTIC_VOXEL_RENDERING_SRC_UTILS_BITS_H

#include <cassert>

namespace pf {

template<typename T>
inline void setBitAtIndex(T &data, std::size_t idx) {
  assert(idx < sizeof(T));
  data |= T{1} << idx;
}

template<typename T>
inline void unSetBitAtIndex(T &data, std::size_t idx) {
  assert(idx < sizeof(T));
  data &= ~(T{1} << idx);
}

template<typename T>
inline void setBitValueAtIndex(T &data, std::size_t idx, bool value) {
  if (value) {
    setBitValueAtIndex(data, idx);
  } else {
    unSetBitAtIndex(data, idx);
  }
}

}// namespace pf

#endif//REALISTIC_VOXEL_RENDERING_SRC_UTILS_BITS_H
