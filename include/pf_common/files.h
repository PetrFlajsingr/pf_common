//
// Created by petr on 1/7/21.
//

#ifndef REALISTIC_VOXEL_RENDERING_SRC_UTILS_FILES_H
#define REALISTIC_VOXEL_RENDERING_SRC_UTILS_FILES_H

#include <filesystem>
#include <pf_common/exceptions/StackTraceException.h>
#include <ranges>
#include <vector>

namespace pf {
/**
 * List all files in folder.
 * @param folder folder to search through
 * @return all regular files in folder (absolute path)
 * @throws InvalidArgumentException when folder is not path to a valid directory
 */
inline std::vector<std::filesystem::path> filesInFolder(const std::filesystem::path &folder) {
  if (!std::filesystem::is_directory(folder)) {
    throw InvalidArgumentException("Path is not a directory: '{}'", folder.string());
  }
  auto result = std::vector<std::filesystem::path>();
  for (const auto &p : std::filesystem::directory_iterator(folder)) {
    if (std::filesystem::is_regular_file(p.path())) { result.emplace_back(p.path()); }
  }
  return result;
}
}// namespace pf

#endif//REALISTIC_VOXEL_RENDERING_SRC_UTILS_FILES_H
