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

template<typename F, typename... Obj>
void iterateTuple(F &&action, std::tuple<Obj...> const &tup) {
  details::iterateTupleImpl(action, tup, std::make_index_sequence<sizeof...(Obj)>());
}

template<typename F, typename... Obj>
void iterateTuplePairs(F &&action, std::tuple<Obj...> const &tup) {
  details::iterateTuplePairsImpl(action, tup, std::make_index_sequence<sizeof...(Obj) - 1>());
}

}// namespace pf
#endif//PF_COMMON_TUPLE_H
