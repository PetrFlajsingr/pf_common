/**
 * @file array.h
 * @brief std::array utilities.
 * @author Petr Flajšingr
 * @date 4.22.21
 */

#ifndef PF_COMMON_ARRAY_H
#define PF_COMMON_ARRAY_H

#include <array>
#include <pf_common/concepts/CommonType.h>
#include <type_traits>

namespace pf {
template<typename T, std::convertible_to<T>... Args>
[[nodiscard]] constexpr std::array<T, sizeof...(Args)> make_array(Args &&...args) {
  return {std::forward<Args>(args)...};
}
}// namespace pf

#endif//PF_COMMON_ARRAY_H
