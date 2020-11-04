//
// Created by petr on 9/24/20.
//

#ifndef PF_COMMON_STREAMCONCEPTS_H
#define PF_COMMON_STREAMCONCEPTS_H

#include <istream>
#include <ostream>
namespace pf {
template<typename T>
concept StreamInputable = requires(T t, std::ostream o) {
  { o << t }
  ->std::convertible_to<std::ostream &>;
};

template<typename T>
concept StreamOutputable = requires(T t, std::istream i) {
  { t >> i }
  ->std::convertible_to<std::istream &>;
};

template<typename T>
concept Streamable = StreamOutputable<T> &&StreamInputable<T>;
}// namespace pf_common
#endif//PF_COMMON_STREAMCONCEPTS_H
