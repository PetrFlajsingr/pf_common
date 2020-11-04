//
// Created by Petr on 19.11.2019.
//

template<unsigned int Dimensions>
float pf::math::BoundingSphere<Dimensions>::distance(BoundingSphere::Point point) const {
  return glm::distance(center, point) - radius;
}
template<unsigned int Dimensions>
pf::math::BoundingSphere<Dimensions>::BoundingSphere(Point center, float radius)
    : center(center), radius(radius) {}
S