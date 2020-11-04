//
// Created by Petr on 19.11.2019.
//

#ifndef PF_COMMON_BOUNDINGSPHERE_H
#define PF_COMMON_BOUNDINGSPHERE_H

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

namespace pf::math {
template<unsigned int Dimensions = 3>
struct BoundingSphere {
  static_assert(Dimensions == 2 || Dimensions == 3);
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
