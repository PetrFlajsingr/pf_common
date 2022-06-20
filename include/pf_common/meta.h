/**
* @file meta.h
* @brief Utilities for metaprogramming.
* @author Petr Flajšingr
* @date 20.6.22
*/

#ifndef PF_COMMON_META_H
#define PF_COMMON_META_H

#include <cstddef>

namespace pf {

template<auto Needle, auto HaystackStart, auto... Haystack>
consteval std::size_t indexInVarArgList() {
  if constexpr (Needle == HaystackStart) {
    return 0;
  } else {
    if constexpr (sizeof...(Haystack) == 0) {
      return -1;
    } else {
      return 1 + indexInVarArgList<Needle, Haystack...>();
    }
  }
}
template<auto Head, auto... Tail>
constexpr auto varArgValueForIndex(std::size_t index, std::size_t currIndex = 0) {
  if (currIndex == index) { return Head; }
  if constexpr (sizeof...(Tail) == 0) {
    return decltype(Head){};
  } else {
    return varArgValueForIndex<Tail...>(index, currIndex + 1);
  }
}

}// namespace pf

#endif//PF_COMMON_META_H
