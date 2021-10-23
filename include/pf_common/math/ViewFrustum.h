//
// Created by Petr on 19.11.2019.
//

#ifndef UTILITIES_VIEWFRUSTUM_H
#define UTILITIES_VIEWFRUSTUM_H

#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "common.h"
#include <array>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>

namespace pf::math {

class ViewFrustum {
  std::array<glm::vec4, 6> planes;

public:
  inline static ViewFrustum FromProjectionView(const glm::mat4 &viewMatrix,
                                        const glm::mat4 &projectionMatrix)   {
    ViewFrustum result;
    const auto &v = viewMatrix;
    const auto &p = projectionMatrix;

    glm::mat4 clipMatrix;

    clipMatrix[0][0] = v[0][0] * p[0][0] + v[0][1] * p[1][0] + v[0][2] * p[2][0] +
        v[0][3] * p[3][0];
    clipMatrix[1][0] = v[0][0] * p[0][1] + v[0][1] * p[1][1] + v[0][2] * p[2][1] +
        v[0][3] * p[3][1];
    clipMatrix[2][0] = v[0][0] * p[0][2] + v[0][1] * p[1][2] + v[0][2] * p[2][2] +
        v[0][3] * p[3][2];
    clipMatrix[3][0] = v[0][0] * p[0][3] + v[0][1] * p[1][3] + v[0][2] * p[2][3] +
        v[0][3] * p[3][3];
    clipMatrix[0][1] = v[1][0] * p[0][0] + v[1][1] * p[1][0] + v[1][2] * p[2][0] +
        v[1][3] * p[3][0];
    clipMatrix[1][1] = v[1][0] * p[0][1] + v[1][1] * p[1][1] + v[1][2] * p[2][1] +
        v[1][3] * p[3][1];
    clipMatrix[2][1] = v[1][0] * p[0][2] + v[1][1] * p[1][2] + v[1][2] * p[2][2] +
        v[1][3] * p[3][2];
    clipMatrix[3][1] = v[1][0] * p[0][3] + v[1][1] * p[1][3] + v[1][2] * p[2][3] +
        v[1][3] * p[3][3];
    clipMatrix[0][2] = v[2][0] * p[0][0] + v[2][1] * p[1][0] + v[2][2] * p[2][0] +
        v[2][3] * p[3][0];
    clipMatrix[1][2] = v[2][0] * p[0][1] + v[2][1] * p[1][1] + v[2][2] * p[2][1] +
        v[2][3] * p[3][1];
    clipMatrix[2][2] = v[2][0] * p[0][2] + v[2][1] * p[1][2] + v[2][2] * p[2][2] +
        v[2][3] * p[3][2];
    clipMatrix[3][2] = v[2][0] * p[0][3] + v[2][1] * p[1][3] + v[2][2] * p[2][3] +
        v[2][3] * p[3][3];
    clipMatrix[0][3] = v[3][0] * p[0][0] + v[3][1] * p[1][0] + v[3][2] * p[2][0] +
        v[3][3] * p[3][0];
    clipMatrix[1][3] = v[3][0] * p[0][1] + v[3][1] * p[1][1] + v[3][2] * p[2][1] +
        v[3][3] * p[3][1];
    clipMatrix[2][3] = v[3][0] * p[0][2] + v[3][1] * p[1][2] + v[3][2] * p[2][2] +
        v[3][3] * p[3][2];
    clipMatrix[3][3] = v[3][0] * p[0][3] + v[3][1] * p[1][3] + v[3][2] * p[2][3] +
        v[3][3] * p[3][3];

    result.planes[0].x = clipMatrix[3][0] - clipMatrix[0][0];
    result.planes[0].y = clipMatrix[3][1] - clipMatrix[0][1];
    result.planes[0].z = clipMatrix[3][2] - clipMatrix[0][2];
    result.planes[0].w = clipMatrix[3][3] - clipMatrix[0][3];

    result.planes[1].x = clipMatrix[3][0] + clipMatrix[0][0];
    result.planes[1].y = clipMatrix[3][1] + clipMatrix[0][1];
    result.planes[1].z = clipMatrix[3][2] + clipMatrix[0][2];
    result.planes[1].w = clipMatrix[3][3] + clipMatrix[0][3];

    result.planes[2].x = clipMatrix[3][0] + clipMatrix[1][0];
    result.planes[2].y = clipMatrix[3][1] + clipMatrix[1][1];
    result.planes[2].z = clipMatrix[3][2] + clipMatrix[1][2];
    result.planes[2].w = clipMatrix[3][3] + clipMatrix[1][3];

    result.planes[3].x = clipMatrix[3][0] - clipMatrix[1][0];
    result.planes[3].y = clipMatrix[3][1] - clipMatrix[1][1];
    result.planes[3].z = clipMatrix[3][2] - clipMatrix[1][2];
    result.planes[3].w = clipMatrix[3][3] - clipMatrix[1][3];

    result.planes[4].x = clipMatrix[3][0] - clipMatrix[2][0];
    result.planes[4].y = clipMatrix[3][1] - clipMatrix[2][1];
    result.planes[4].z = clipMatrix[3][2] - clipMatrix[2][2];
    result.planes[4].w = clipMatrix[3][3] - clipMatrix[2][3];

    result.planes[5].x = clipMatrix[3][0] + clipMatrix[2][0];
    result.planes[5].y = clipMatrix[3][1] + clipMatrix[2][1];
    result.planes[5].z = clipMatrix[3][2] + clipMatrix[2][2];
    result.planes[5].w = clipMatrix[3][3] + clipMatrix[2][3];

    for (int i = 0; i < 6; i++) {
      result.planes[i] = glm::normalize(result.planes[i]);
    }
    return result;
  }

  [[nodiscard]] RelativePosition contains(const BoundingBox<3> &aabb) const{
    static auto createPoint = [](auto &first, auto &second, auto &normal) {
      glm::vec3 result = first;

      if (normal.x >= 0.0f)
        result.x = second.x;
      if (normal.y >= 0.0f)
        result.y = second.y;
      if (normal.z >= 0.0f)
        result.z = second.z;

      return result;
    };
    for (auto i = 0; i < 6; i++) {
      const float pos = planes[i].w;
      const auto normal = glm::vec3(planes[i]);

      if (glm::dot(normal, createPoint(aabb.p1, aabb.p2, normal)) + pos < 0.0f) {
        return RelativePosition::Outside;
      }

      if (glm::dot(normal, createPoint(aabb.p2, aabb.p1, normal)) + pos < 0.0f) {
        return RelativePosition::Intersection;
      }
    }
    return RelativePosition::Inside;
  }
};
}// namespace pf::math

#endif // UTILITIES_VIEWFRUSTUM_H
