//
// Created by petr on 9/24/20.
//

#ifndef VOXEL_RENDER_SUBSCRIPTION_H
#define VOXEL_RENDER_SUBSCRIPTION_H

#include <functional>

namespace pf {
class Subscription {
 public:
  using Unsubscriber = std::function<void()>;
  inline explicit Subscription(Unsubscriber &&unsubscriber)
      : unsub(unsubscriber) {}
  Subscription(const Subscription &) = delete;
  Subscription &operator=(const Subscription &) = delete;
  inline Subscription(Subscription &&other) noexcept
      : unsub(std::move(other.unsub)) {
    other.unsub = [] {};
  }
  inline Subscription &operator=(Subscription &&other) noexcept {
    unsub = other.unsub;
    other.unsub = [] {};
    return *this;
  }

  inline void unsubscribe() { unsub(); }

 private:
  Unsubscriber unsub;
};
}// namespace pf

#endif//VOXEL_RENDER_SUBSCRIPTION_H
