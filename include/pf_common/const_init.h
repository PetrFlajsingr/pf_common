//
// Created by petr on 12/5/20.
//

#ifndef PF_COMMON_INCLUDE_PF_COMMON_CONST_INIT_H
#define PF_COMMON_INCLUDE_PF_COMMON_CONST_INIT_H

/**
 * Shorthand for:
 * auto value = [&] {
 *  auto result = type{};
 *  // type setup
 *  return result;
 * }();
 * |
 * V
 * auto value = CONST_INIT(
 *              type,
 *              // type setup
 *              )
 */
#define CONST_INIT(type, init)                                                                                         \
  [&] {                                                                                                                \
    auto result = type{};                                                                                              \
    init return result;                                                                                                \
  }()

#endif//PF_COMMON_INCLUDE_PF_COMMON_CONST_INIT_H
