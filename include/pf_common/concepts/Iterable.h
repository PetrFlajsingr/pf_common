//
// Created by petr on 9/24/20.
//

#ifndef VOXEL_RENDER_ITERABLE_H
#define VOXEL_RENDER_ITERABLE_H

#include <iterator>
namespace pf {
template<typename T>
concept Iterable = requires(T t) {
  { std::begin(t) }
  ->std::forward_iterator;
  { std::end(t) }
  ->std::sentinel_for<decltype(std::begin(t))>;
};

template<typename T, typename ValueType>
concept Iterable_of = Iterable<T> &&requires(T t) {
  { *std::begin(t) }
  ->std::convertible_to<ValueType>;
};

template<Iterable T>
using IterableValueType = decltype(*std::begin(std::declval<T>()));
}// namespace pf_common
#endif//VOXEL_RENDER_ITERABLE_H
