/**
* @file BoundingBox.h
* @brief Simple 2D or 3D bounding box.
* @author Petr Flajšingr
* @date 19.11.19
*/
#ifndef PF_COMMON_MATH_BOUNDINGBOX_H
#define PF_COMMON_MATH_BOUNDINGBOX_H

#include "common.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
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
 BoundingBox(Point p1, Point p2) : p1(p1), p2(p2) {}

 bool operator==(const BoundingBox &rhs) const {
   return p1 == rhs.p1 && p2 == rhs.p2;
 }


 bool operator!=(const BoundingBox &rhs) const {
   return !(rhs == *this);
 }

 /**
  * Check relative position of another BoundingBox.
  * @param aabb other bounding box
  * @return relative position
  * @see RelativePosition
  */
 [[nodiscard]] RelativePosition contains(const BoundingBox &aabb) const {
   if constexpr (Dimensions == 2) {
     const bool minIn = p1.x < aabb.p1.x && p1.y < aabb.p1.y && p2.x > aabb.p1.x && p2.y > aabb.p1.y;
     const bool maxIn = p2.x > aabb.p2.x && p2.y > aabb.p2.y && p1.x < aabb.p2.x && p1.y < aabb.p2.y;
     if (minIn && maxIn) { return RelativePosition::Inside; }
     if (minIn || maxIn) { return RelativePosition::Intersection; }
     return RelativePosition::Outside;
   } else {
     const bool minIn = p1.x < aabb.p1.x && p1.y < aabb.p1.y && p1.z < aabb.p1.z && p2.x > aabb.p1.x && p2.y > aabb.p1.y
         && p2.z > aabb.p1.z;
     const bool maxIn = p2.x > aabb.p2.x && p2.y > aabb.p2.y && p2.z > aabb.p2.z && p1.x < aabb.p2.x && p1.y < aabb.p2.y
         && p1.z < aabb.p2.z;
     if (minIn && maxIn) { return RelativePosition::Inside; }
     if (minIn || maxIn) { return RelativePosition::Intersection; }
     return RelativePosition::Outside;
   }
 }

 /**
  * Check whether the bounding box contains a point or not.
  * @param point point to check for
  * @return true if the point is contained within the bounding box, false otherwise
  */
 [[nodiscard]] bool contains(Point point) const{
   if constexpr (Dimensions == 2) {
     return p1.x <= point.x && p1.y <= point.y && point.x <= p2.x && point.y <= p2.y;
   } else {
     return p1.x <= point.x && p1.y <= point.y && p1.z <= point.z && point.x <= p2.x && point.y <= p2.y
         && point.z <= p2.z;
   }
 }

 [[nodiscard]] float width() const { return p2.x - p1.x; }

 [[nodiscard]] float height() const { return p2.y - p1.y; }

 [[nodiscard]] float depth() const requires(Dimensions == 3) { return p2.z - p1.z; }

 inline friend BoundingBox operator*(const glm::mat3 &m, const BoundingBox &self) requires(Dimensions == 2) {
   return BoundingBox{(m * glm::vec3{self.p1, 1}).xy(), (m * glm::vec3{self.p2, 1}).xy()};
 }
 inline friend BoundingBox operator*(const glm::mat4 &m, const BoundingBox &self) requires(Dimensions == 3) {
   return BoundingBox{(m * glm::vec4{self.p1, 1}).xyz(), (m * glm::vec4{self.p2, 1}).xyz()};
 }

 inline friend BoundingBox &operator*=(const glm::mat3 &m, BoundingBox &self) requires(Dimensions == 2) {
   self = m * self;
   return self;
 }
 inline friend BoundingBox &operator*=(const glm::mat4 &m, BoundingBox &self) requires(Dimensions == 3) {
   self = m * self;
   return self;
 }

 [[nodiscard]] float distance(const BoundingBox &other);

 [[nodiscard]] BoundingBox combine(const BoundingBox &other) const;
};

template<>
inline float BoundingBox<2>::distance(const BoundingBox<2> &other) {
 const auto outterBB = BoundingBox<2>{{glm::min(p1.x, other.p1.x), glm::min(p1.y, other.p1.y)},
                                      {glm::max(p2.x, other.p2.x), glm::max(p2.y, other.p2.y)}};
 const auto innerWidth = outterBB.width() - width() - other.width();
 const auto innerHeight = outterBB.height() - height() - other.height();
 const auto distance = glm::sqrt(glm::pow(innerWidth, 2) + glm::pow(innerHeight, 2));
 return distance;
}

template<>
inline float BoundingBox<3>::distance(const BoundingBox<3> &other) {
 const auto outterBB =
     BoundingBox<3>{{glm::min(p1.x, other.p1.x), glm::min(p1.y, other.p1.y), glm::min(p1.z, other.p1.z)},
                    {glm::max(p2.x, other.p2.x), glm::max(p2.y, other.p2.y), glm::max(p2.z, other.p2.z)}};
 const auto innerWidth = outterBB.width() - width() - other.width();
 const auto innerHeight = outterBB.height() - height() - other.height();
 const auto innerDepth = outterBB.depth() - depth() - other.depth();
 const auto distance = glm::sqrt(glm::pow(innerWidth, 2) + glm::pow(innerHeight, 2) + glm::pow(innerDepth, 2));
 return distance;
}

template<>
inline BoundingBox<2> BoundingBox<2>::combine(const BoundingBox<2> &other) const {
 return BoundingBox<2>{{glm::min(p1.x, other.p1.x), glm::min(p1.y, other.p1.y)},
                       {glm::max(p2.x, other.p2.x), glm::max(p2.y, other.p2.y)}};
}

template<>
inline BoundingBox<3> BoundingBox<3>::combine(const BoundingBox<3> &other) const {
 return BoundingBox<3>{{glm::min(p1.x, other.p1.x), glm::min(p1.y, other.p1.y), glm::min(p1.z, other.p1.z)},
                       {glm::max(p2.x, other.p2.x), glm::max(p2.y, other.p2.y), glm::max(p2.z, other.p2.z)}};
}

template<unsigned int Dims>
std::ostream &operator<<(std::ostream &stream, const BoundingBox<Dims> &aabb) {
 stream << "P1: [" << aabb.p1.x << ", " << aabb.p1.y << ", " << aabb.p1.z << "], P2: [ " << aabb.p2.x << ", "
        << aabb.p2.y << ", " << aabb.p2.z << "]";
 return stream;
}
}// namespace pf::math

#endif// PF_COMMON_MATH_BOUNDINGBOX_H
