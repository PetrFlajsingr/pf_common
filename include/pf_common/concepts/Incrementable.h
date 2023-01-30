/**
 * @file Incrementable.h
 * @brief Provides concepts to check for increment capabilities of a type.
 * @author Petr Flajšingr
 * @date 24.9.20
 */

#ifndef PF_COMMON_CONCEPTS_INCREMENTABLE_H
#define PF_COMMON_CONCEPTS_INCREMENTABLE_H

#include <concepts>

namespace pf {
/**
 * @brief A type which can use all increment operators.
 * @tparam T
 */
template<typename T>
concept Incrementable = requires(T t) {
                          { ++t };
                          { t += t };
                          { t + t } -> std::convertible_to<T>;
                        };

/**
 * @brief A type which is capable of using the modulo operator.
 * @tparam T
 */
template<typename T>
concept ModCapable = requires(T t) {
                       { t % t } -> std::convertible_to<T>;
                     };
}// namespace pf
#endif//PF_COMMON_CONCEPTS_INCREMENTABLE_H
