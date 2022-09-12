/**
 * @file bits.h
 * @brief Functions for memory bit manipulation
 * @author Petr Flajšingr
 * @date 2.1.21
 */
#ifndef PF_COMMON_BITS_H
#define PF_COMMON_BITS_H

#include <cassert>
#include <cstddef>

namespace pf {

/**
 * Set bit value to 1 at given index.
 * @tparam T manipulated type
 * @param data data to alter
 * @param idx index of bit
 */
template<typename T>
inline void setBitAtIndex(T &data, std::size_t idx) {
  assert(idx < sizeof(T));
  data |= T{1} << idx;
}

/**
 * Set bit value to 0 at given index.
 * @tparam T manipulated type
 * @param data data to alter
 * @param idx index of bit
 */
template<typename T>
inline void unSetBitAtIndex(T &data, std::size_t idx) {
  assert(idx < sizeof(T));
  data &= ~(T{1} << idx);
}

/**
 * Set bit value at given index.
 * @tparam T manipulated type
 * @param data data to alter
 * @param idx index of bit
 * @param value value to set
 */
template<typename T>
inline void setBitValueAtIndex(T &data, std::size_t idx, bool value) {
  if (value) {
    setBitValueAtIndex(data, idx);
  } else {
    unSetBitAtIndex(data, idx);
  }
}

}// namespace pf

#endif//PF_COMMON_BITS_H
