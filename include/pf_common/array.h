//
// Created by petr on 4/22/21.
//

#ifndef PF_COMMON_INCLUDE_PF_COMMON_ARRAY_H
#define PF_COMMON_INCLUDE_PF_COMMON_ARRAY_H

#include <array>
#include <type_traits>

namespace pf {
template<typename... Args>
constexpr std::array<std::common_type_t<Args...>, sizeof...(Args)> make_array(Args &&...args) {
  return {std::forward<Args>(args)...};
}
}// namespace pf

#endif//PF_COMMON_INCLUDE_PF_COMMON_ARRAY_H
