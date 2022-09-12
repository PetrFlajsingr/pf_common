/**
 * @file always_false.h
 * @brief Simple constexpr always false value.
 * @author Petr Flajšingr
 * @date 6.6.22
 */

#ifndef PF_COMMON_ALWAYS_FALSE_H
#define PF_COMMON_ALWAYS_FALSE_H

namespace pf {
template<typename... T>
constexpr bool always_false = false;
}

#endif//PF_COMMON_ALWAYS_FALSE_H
