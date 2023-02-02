/**
 * @file Visitor.h
 * @brief Utility for lambda overloading.
 * @author Petr Flajšingr
 * @date 1.11.20
 */
#ifndef PF_COMMON_VISITOR_H
#define PF_COMMON_VISITOR_H

namespace pf {

/**
 * @brief Overloading of multiple callables via inheritance.
 * @tparam Ts callables
 */
template<typename... Ts>
struct Visitor : Ts... {
  using Ts::operator()...;
};
template<typename... Ts>
Visitor(Ts...) -> Visitor<Ts...>;

}// namespace pf
#endif//PF_COMMON_VISITOR_H
