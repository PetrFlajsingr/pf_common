//
// Created by Petr on 19.11.2019.
//

#ifndef PF_COMMON_BOUNDINGBOX_H
#define PF_COMMON_BOUNDINGBOX_H

#include "common.h"
#include <glm/glm.hpp>
#include <ostream>
#include <pf_common/concepts/OneOf.h>

namespace pf::math {
/**
 * @brief Axis aligned bounding box.
 * @tparam Dimensions 2D or 3D
 */
template<unsigned int Dimensions = 3>
requires(OneOfValues_v<Dimensions, 2, 3>) struct BoundingBox {
  using Point = std::conditional_t<Dimensions == 3, glm::vec3, glm::vec2>;
  Point p1;
  Point p2;

  BoundingBox() = default;
  /**
   * Construct BoundingBox.
   * @param p1 first point
   * @param p2 second point
   */
  BoundingBox(glm::vec3 p1, glm::vec3 p2) : p1(p1), p2(p2) {}

  bool operator==(const BoundingBox &rhs) const;

  bool operator!=(const BoundingBox &rhs) const;

  /**
   * Check relative position of another BoundingBox.
   * @param aabb other bounding box
   * @return relative position
   * @see RelativePosition
   */
  [[nodiscard]] RelativePosition contains(const BoundingBox &aabb) const;

  /**
   * Check whether the bounding box contains a point or not.
   * @param point point to check for
   * @return true if the point is contained within the bounding box, false otherwise
   */
  [[nodiscard]] bool contains(Point point) const;
};

template<unsigned int Dims>
std::ostream &operator<<(std::ostream &stream, const BoundingBox<Dims> &aabb);
}// namespace pf::math

#include "BoundingBox.tpp"
#endif// PF_COMMON_BOUNDINGBOX_H
