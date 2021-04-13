//
// Created by petr on 1/2/21.
//

#ifndef REALISTIC_VOXEL_RENDERING_SRC_UTILS_INTERFACE_SERIALIZABLE_H
#define REALISTIC_VOXEL_RENDERING_SRC_UTILS_INTERFACE_SERIALIZABLE_H

#include <filesystem>
#include <fstream>
#include <pf_common/exceptions/StackTraceException.h>

namespace pf {

template<typename T>
concept Serializable = requires(const T t) {
  { t.serialize() } -> std::same_as<std::vector<std::byte>>;
};
template<typename T>
concept Deserializable = requires(std::span<const std::byte> data) {
  { T::Deserialize(data) } -> std::same_as<T>;
};

std::vector<std::byte> serialize(const Serializable auto &obj) { return obj.serialize(); }

template<Deserializable T>
T deserialize(std::span<const std::byte> data) {
  return T::Deserialize(data);
}

void saveToFile(const std::filesystem::path &path, const Serializable auto &obj) {
  const auto data = obj.serialize();
  auto ostream = std::ofstream(path, std::ios::binary);
  ostream.write(reinterpret_cast<const char *>(data.data()), data.size());
}

template<Deserializable T>
T loadFromFile(const std::filesystem::path &path) {
  auto istream = std::ifstream(path, std::ios::binary | std::ios::ate);
  const auto size = istream.tellg();
  istream.seekg(0, std::ios::beg);

  auto data = std::vector<std::byte>(size);
  if (istream.read(reinterpret_cast<char *>(data.data()), size)) { return T::Deserialize(data); }
  throw StackTraceException::fmt("Could not deserialize from '{}'", path.string());
}

}// namespace pf
#endif//REALISTIC_VOXEL_RENDERING_SRC_UTILS_INTERFACE_SERIALIZABLE_H
