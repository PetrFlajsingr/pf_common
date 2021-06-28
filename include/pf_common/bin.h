/**
 * @file bin.h
 * @brief Functions for raw data conversion.
 * @author Petr Flajšingr
 * @date 4.1.21
 */
#ifndef PF_COMMON_BIN_H
#define PF_COMMON_BIN_H

#include "concepts/Serializable.h"

namespace pf {
/**
 * Raw type to binary data conversion.
 * @tparam T type to convert
 * @param data data to convert
 * @return data as binary data
 */
template<typename T>
std::vector<std::byte> toBytes(const T &data) {
  if constexpr (Serializable<T>) { return data.serialize(); }
  auto result = std::vector<std::byte>(sizeof(data));
  std::ranges::copy(std::span(reinterpret_cast<const std::byte *>(&data), sizeof(data)), result.begin());
  return result;
}

/**
 * Reconstruct T from provided binary data.
 * @tparam T type to reconstruct
 * @param data binary input data
 * @return reconstructed value
 */
template<typename T>
requires std::is_default_constructible_v<T> T fromBytes(std::span<const std::byte> data) {
  assert(data.size() == sizeof(T));
  auto result = T{};
  std::ranges::copy(data, reinterpret_cast<std::byte *>(&result));
  return result;
}

}// namespace pf

#endif//PF_COMMON_BIN_H
