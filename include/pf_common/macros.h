//
// Created by xflajs00 on 04.02.2023.
//

#ifndef PF_COMMON_MACROS_H
#define PF_COMMON_MACROS_H

#if defined(_MSC_VER)
#define PF_FORCEINLINE __forceinline
#else
#define PF_FORCEINLINE __attribute__((always_inline)) inline
#endif

#if defined(_MSC_VER)
#define PF_NOINLINE __declspec(noinline)
#else
#define PF_NOINLINE __attribute__((noinline))
#endif

#if defined(_MSC_VER)
#define PF_DEBUG_BREAK() (__nop(), __debugbreak())
#else
#define PF_DEBUG_BREAK() raise(SIGTRAP)
#endif

#endif//PF_COMMON_MACROS_H
