//
// Created by Petr on 19.11.2019.
//

#ifndef PF_COMMON_BOUNDINGSPHERE_H
#define PF_COMMON_BOUNDINGSPHERE_H

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <pf_common/concepts/OneOf.h>

namespace pf::math {
template<unsigned int Dimensions = 3>
requires(OneOfValues_v<Dimensions, 2, 3>) struct BoundingSphere {
  using Point = std::conditional_t<Dimensions == 3, glm::vec3, glm::vec2>;
  Point center;
  float radius;

  BoundingSphere() = default;
  BoundingSphere(Point center, float radius);

  [[nodiscard]] float distance(Point point) const;
};
}// namespace pf::math

#include "BoundingSphere.tpp"
#endif// PF_COMMON_BOUNDINGSPHERE_H
