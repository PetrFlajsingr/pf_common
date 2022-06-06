//
// Created by Petr on 6/6/2022.
//

#ifndef PF_COMMON_MANDATORYVALUE_H
#define PF_COMMON_MANDATORYVALUE_H

#include <pf_common/always_false.h>

namespace pf {
/**
 * @brief Type to be used for aggregate init struct members, which need to be provided by the user.
 * @code
template \<typename = void\>
struct Args {
    std::string noDefault = MandatoryValue{};
    int hasDefault = 1;
};
 * @endcode
 */
struct MandatoryValue {
  template<typename T>
  operator T() {
    static_assert(always_false<T>, "You need to provide an argument");
    return T{};
  }
};
}// namespace pf

#endif//PF_COMMON_MANDATORYVALUE_H
