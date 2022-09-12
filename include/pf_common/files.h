/**
 * @file file.h
 * @brief Utility functions for working with filesystem.
 * @author Petr Flajšingr
 * @date 7.1.21
 */
#ifndef PF_COMMON_FILES_H
#define PF_COMMON_FILES_H

#include <filesystem>
#include <optional>
#include <ranges>
#include <vector>

namespace pf {
/**
 * List all files in folder.
 * @param folder folder to search through
 * @return all regular files in folder (absolute path), std::nullopt on fail
 */
inline std::optional<std::vector<std::filesystem::path>> filesInFolder(const std::filesystem::path &folder) {
  if (!std::filesystem::is_directory(folder)) { return std::nullopt; }
  auto result = std::vector<std::filesystem::path>();
  for (const auto &p : std::filesystem::directory_iterator(folder)) {
    if (std::filesystem::is_regular_file(p.path())) { result.emplace_back(p.path()); }
  }
  return result;
}
}// namespace pf

#endif//PF_COMMON_FILES_H
