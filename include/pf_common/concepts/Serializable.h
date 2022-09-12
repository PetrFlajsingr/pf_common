/**
 * @file Serializable.h
 * @brief Concepts for de/serializable objects and utility functions for de/serialization.
 * @author Petr Flajšingr
 * @date 2.1.21
 */
#ifndef PF_COMMON_SERIALIZABLE_H
#define PF_COMMON_SERIALIZABLE_H

#include <filesystem>
#include <fstream>
#include <optional>
#include <span>

namespace pf {

/**
 * @brief An object which provides serialisation to binary data.
 * @tparam T
 */
template<typename T>
concept Serializable = requires(const T t) {
                         { t.serialize() } -> std::same_as<std::vector<std::byte>>;
                       };
/**
 * @brief An object which provides deserialization from binary data.
 * @tparam T
 */
template<typename T>
concept Deserializable = requires(std::span<const std::byte> data) {
                           { T::Deserialize(data) } -> std::same_as<T>;
                         };

/**
 * Convenience function for object serialisation.
 * @param obj object to be serialised
 * @return binary data
 */
std::vector<std::byte> serialize(const Serializable auto &obj) { return obj.serialize(); }

/**
 * Convenience function for object deserialization.
 * @param data binary data
 * @return deserialized object.
 * @tparam T type to deserialize
 */
template<Deserializable T>
T deserialize(std::span<const std::byte> data) {
  return T::Deserialize(data);
}

/**
 * Save serializable object to file.
 * @param path path to save data into
 * @param obj object to serialize
 */
void saveToFile(const std::filesystem::path &path, const Serializable auto &obj) {
  const auto data = obj.serialize();
  auto ostream = std::ofstream(path, std::ios::binary);
  ostream.write(reinterpret_cast<const char *>(data.data()), data.size());
}

/**
 * Deserialize an object from file data.
 * @tparam T type to be deserialized
 * @param path path to source file
 * @return deserialized object, nullopt if fail
 */
template<Deserializable T>
std::optional<T> loadFromFile(const std::filesystem::path &path) {
  auto istream = std::ifstream(path, std::ios::binary | std::ios::ate);
  const auto size = istream.tellg();
  istream.seekg(0, std::ios::beg);

  auto data = std::vector<std::byte>(size);
  if (istream.read(reinterpret_cast<char *>(data.data()), size)) { return T::Deserialize(data); }
  return std::nullopt;
}

}// namespace pf
#endif//PF_COMMON_SERIALIZABLE_H
