//
// Created by xflajs00 on 04.02.2023.
//

#ifndef PF_COMMON_MACROS_H
#define PF_COMMON_MACROS_H

#if defined(__clang__)
#define PF_CLANG 1
#define PF_MSVC 0
#define PF_GCC 0
#elif defined(__GNUC__) || defined(__GNUG__)
#define PF_CLANG 0
#define PF_MSVC 0
#define PF_GCC 1
#elif defined(_MSC_VER)
#define PF_CLANG 0
#define PF_MSVC 1
#define PF_GCC 0
#else
#error Unsupported compiler
#endif

#if PF_MSVC
#define PF_CPLUSPLUS_VERSION _MSVC_LANG
#else
#define PF_CPLUSPLUS_VERSION __cplusplus
#endif

#if PF_MSVC
#define PF_FORCEINLINE __forceinline
#else
#define PF_FORCEINLINE __attribute__((always_inline)) inline
#endif

#if PF_MSVC
#define PF_NOINLINE __declspec(noinline)
#else
#define PF_NOINLINE __attribute__((noinline))
#endif

#if PF_MSVC
#define PF_DEBUG_BREAK() (__nop(), __debugbreak())
#else
#define PF_DEBUG_BREAK() raise(SIGTRAP)
#endif

// TODO: replace usage with [[no_unique_address]] once msvc supports it
#if PF_MSVC
#define PF_NOUNIQUEADDRESS [[msvc::no_unique_address]]
#else
#define PF_NOUNIQUEADDRESS [[no_unique_address]]
#endif

// TODO: replace usage of this with [[assume(x)]] in c++23
#if PF_MSVC
#define PF_ASSUME(...) __assume(__VA_ARGS__)
#elif PF_GCC
#define PF_ASSUME(...)                                                                                                                     \
  do {                                                                                                                                     \
    if (!(__VA_ARGS__)) { __builtin_unreachable(); }                                                                                       \
  } while (0)
#elif PF_CLANG
#define PF_ASSUME(...) __builtin_assume(__VA_ARGS__)
#endif

#endif//PF_COMMON_MACROS_H
