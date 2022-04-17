//
// Created by xflajs00 on 17.04.2022.
//

#ifndef PF_COMMON_TYPE_NAME_H
#define PF_COMMON_TYPE_NAME_H

#include <algorithm>
#include <array>
#include <string_view>

namespace pf {
template<typename T>
[[nodiscard]] constexpr std::string_view RawTypeName() {
#ifndef _MSC_VER
  return __PRETTY_FUNCTION__;
#else
  return __FUNCSIG__;
#endif
}

struct TypeNameFormat {
  std::size_t junk_leading = 0;
  std::size_t junk_total = 0;
};

constexpr TypeNameFormat type_name_format = [] {
  TypeNameFormat ret;
  std::string_view sample = RawTypeName<int>();
  ret.junk_leading = sample.find("int");
  ret.junk_total = sample.size() - 3;
  return ret;
}();
static_assert(type_name_format.junk_leading != std::size_t(-1),
              "Unable to determine the type name format on this compiler.");

template<typename T>
static constexpr auto type_name_storage = [] {
  std::array<char, RawTypeName<T>().size() - type_name_format.junk_total + 1> ret{};
  std::copy_n(RawTypeName<T>().data() + type_name_format.junk_leading, ret.size() - 1, ret.data());
  return ret;
}();

template<typename T>
[[nodiscard]] constexpr std::string_view TypeName() {
  return {type_name_storage<T>.data(), type_name_storage<T>.size() - 1};
}

template<typename T>
[[nodiscard]] constexpr const char *TypeNameCstr() {
  return type_name_storage<T>.data();
}
}// namespace pf

#endif//PF_COMMON_TYPE_NAME_H
