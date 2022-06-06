//
// Created by Petr on 6/6/2022.
//

#ifndef PF_COMMON_ALWAYS_FALSE_H
#define PF_COMMON_ALWAYS_FALSE_H

namespace pf {
template<typename... T>
constexpr bool always_false = false;
}

#endif//PF_COMMON_ALWAYS_FALSE_H
