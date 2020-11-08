//
// Created by petr on 9/24/20.
//

#ifndef PF_COMMON_SUBSCRIPTION_H
#define PF_COMMON_SUBSCRIPTION_H

#include <functional>

namespace pf {
class Subscription {
 public:
  using Unsubscriber = std::function<void()>;
  inline explicit Subscription(Unsubscriber &&unsubscriber) : unsub(unsubscriber) {}
  Subscription(const Subscription &) = delete;
  Subscription &operator=(const Subscription &) = delete;
  inline Subscription(Subscription &&other) noexcept : unsub(std::move(other.unsub)) {
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

#endif//PF_COMMON_SUBSCRIPTION_H
