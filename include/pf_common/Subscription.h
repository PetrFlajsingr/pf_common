/**
 * @file Subscription.h
 * @brief Object for event unsubscribtion.
 * @author Petr Flajšingr
 * @date 24.9.20
 */
#ifndef PF_COMMON_SUBSCRIPTION_H
#define PF_COMMON_SUBSCRIPTION_H

#include <concepts>
#include <functional>
#include <utility>

namespace pf {
/**
 * @brief Object providing an option to unsubscribe from observing something.
 */
class Subscription {
 public:
  using Unsubscriber = std::function<void()>;
  /**
   * Construct Subscription.
   * @param unsubscriber function doing the unsubscription
   */
  inline explicit Subscription(std::invocable auto &&unsubscriber) : unsub(unsubscriber) {}
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

  /**
   * Unsubscribe.
   */
  inline void unsubscribe() { unsub(); }

 private:
  Unsubscriber unsub;
};
}// namespace pf

#endif//PF_COMMON_SUBSCRIPTION_H
