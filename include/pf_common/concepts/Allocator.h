//
// Created by xflajs00 on 02.02.2023.
//

#ifndef PF_COMMON_ALLOCATOR_H
#define PF_COMMON_ALLOCATOR_H

#include <concepts>

namespace pf {

template<typename T>
concept Allocator = requires(T t, T t2, std::size_t n, typename T::value_type *ptr) {
  typename T::value_type;
  { t == t2 } noexcept -> std::convertible_to<bool>;
  { t != t2 } noexcept -> std::convertible_to<bool>;
  { T{t} } noexcept -> std::same_as<T>;
  { T{std::move(t)} } noexcept -> std::same_as<T>;
  { t.allocate(n) } -> std::same_as<typename T::value_type *>;
  { t.deallocate(ptr, n) };
};

}

#endif//PF_COMMON_ALLOCATOR_H
