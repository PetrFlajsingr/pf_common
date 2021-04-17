/**
 * @file file.h
 * @brief Utility functions for working with filesystem.
 * @author Petr Flajšingr
 * @date 7.1.21
 */
#ifndef PF_COMMON_FILES_H
#define PF_COMMON_FILES_H

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

#endif//PF_COMMON_FILES_H
