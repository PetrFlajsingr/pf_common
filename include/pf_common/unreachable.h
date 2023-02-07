//
// Created by xflajs00 on 07.02.2023.
//

#ifndef PF_COMMON_UNREACHABLE_H
#define PF_COMMON_UNREACHABLE_H

#include <pf_common/macros.h>

namespace pf {

// TODO: replace with std::unreachable in c++23
[[noreturn]] inline void unreachable() {
#if PF_MSVC
    __assume(false);
#elif PF_GCC
    __builtin_unreachable();
#elif PF_CLANG
    __builtin_unreachable();
#endif
}

}

#endif//PF_COMMON_UNREACHABLE_H
