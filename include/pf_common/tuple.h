//
// Created by petr on 10/26/20.
//

#ifndef PF_COMMON_TUPLE_H
#define PF_COMMON_TUPLE_H

#include <tuple>
namespace pf {
namespace details {
template<typename F, typename T, std::size_t... Index>
void iterateTupleImpl(F &&action, T const &tup, std::index_sequence<Index...> const &) {
  [[maybe_unused]] bool ignore[] = {(action(std::get<Index>(tup)), true)...};
}
template<typename F, typename T, std::size_t... Index>
void iterateTuplePairsImpl(F &&action, T const &tup, std::index_sequence<Index...> const &) {
  [[maybe_unused]] bool ignore[] = {(action(std::get<Index>(tup), std::get<Index + 1>(tup)), true)...};
}
}// namespace details

/**
 * Iterate over each element of a tuple and call action on it.
 * @tparam F callable
 * @tparam Obj value types in tuple
 * @param action action to call on each tuple element
 * @param tup tuple
 */
template<typename F, typename... Obj>
void iterateTuple(F &&action, std::tuple<Obj...> const &tup) {
  details::iterateTupleImpl(action, tup, std::make_index_sequence<sizeof...(Obj)>());
}

/**
 * Iterate over each pair of adjacent elements of a tuple and call action on them.
 * @tparam F callable
 * @tparam Obj value types in tuple
 * @param action action to call on each tuple element pair
 * @param tup tuple
 */
template<typename F, typename... Obj>
void iterateTuplePairs(F &&action, std::tuple<Obj...> const &tup) {
  details::iterateTuplePairsImpl(action, tup, std::make_index_sequence<sizeof...(Obj) - 1>());
}

}// namespace pf
#endif//PF_COMMON_TUPLE_H
