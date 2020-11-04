//
// Created by petr on 11/1/20.
//

#ifndef PF_COMMON_UTILS_VISITOR_H
#define PF_COMMON_UTILS_VISITOR_H

namespace pf {
template<typename... Ts>
struct Visitor : Ts... { using Ts::operator()...; };
template<class... Ts>
Visitor(Ts...) -> Visitor<Ts...>;

}// namespace pf
#endif//PF_COMMON_UTILS_VISITOR_H
