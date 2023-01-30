/**
 * @file StreamConcepts.h
 * @brief Concepts for checking type's stream interface.
 * @author Petr Flajšingr
 * @date 24.9.20
 */
#ifndef PF_COMMON_CONCEPTS_STREAMCONCEPTS_H
#define PF_COMMON_CONCEPTS_STREAMCONCEPTS_H

#include <istream>
#include <ostream>

namespace pf {
/**
 * @brief A type which supports << to stream.
 * @tparam T
 */
template<typename T>
concept StreamInputable = requires(T t, std::ostream o) {
                            { o << t } -> std::convertible_to<std::ostream &>;
                          };
/**
 * @brief A type which supports >> to stream.
 * @tparam T
 */
template<typename T>
concept StreamOutputable = requires(T t, std::istream i) {
                             { i >> t } -> std::convertible_to<std::istream &>;
                           };
/**
 * A type which supports both >> and << for streams.
 * @tparam T
 */
template<typename T>
concept Streamable = StreamOutputable<T> && StreamInputable<T>;
}// namespace pf
#endif//PF_COMMON_CONCEPTS_STREAMCONCEPTS_H
