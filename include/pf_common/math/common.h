/**
 * @file common.h
 * @brief Common types for pf::math.
 * @author Petr Flajšingr
 * @date 14.12.19
 */
#ifndef PF_COMMON_MATH_COMMON_H
#define PF_COMMON_MATH_COMMON_H

namespace pf::math {
/**
 * Relative position of two objects.
 */
enum class RelativePosition {
  Inside,/**< Object is fully contained */
  Outside, /**< Objects do not share any area/volume */
  Intersection /**< Objects intersect without the checked one being contained inside the other one */
};
}

#endif// PF_COMMON_COMMON_H
