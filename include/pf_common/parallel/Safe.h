//
// Created by Petr on 21.02.2020.
//

#ifndef PF_COMMON_SAFE_H
#define PF_COMMON_SAFE_H

#include <mutex>

namespace pf {
template<typename T, typename Mutex = std::mutex>
class Safe final {
 public:
  enum class AccessType { ReadWrite, ReadOnly };
  template<AccessType = AccessType::ReadWrite>
  class Access;

 private:
  using ReadOnlyAccess = Access<AccessType::ReadOnly>;
  using ReadWriteAccess = Access<AccessType::ReadWrite>;

 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;

  template<typename... Args>
  explicit Safe(Args &&...args) : value(std::forward<Args &&...>(args)...) {}
  template<typename... Args>
  explicit Safe(Mutex &&mtx, Args &&...args) : mtx(std::move(mtx)), value(std::forward<Args &&...>(args)...) {}
  Safe(const Safe &other) {
    value = other.value;
    mtx = std::mutex{};
  }
  Safe &operator=(const Safe &other) {
    if (this == &other) { return *this; }
    value = other.value;
    mtx = std::mutex{};
    return *this;
  }
  Safe(Safe &&other) = delete;
  Safe &operator=(Safe &&other) = delete;

  ReadWriteAccess get() { return ReadWriteAccess{value, mtx}; }
  ReadOnlyAccess get() const { return ReadOnlyAccess{value, mtx}; }

  ReadWriteAccess writeAccess() { return ReadWriteAccess{value, mtx}; }
  ReadOnlyAccess readOnlyAccess() const { return ReadOnlyAccess{value, mtx}; }

  ReadWriteAccess operator->() { return get(); }
  ReadOnlyAccess operator->() const { return get(); }

  reference unsafe() { return value; }
  const_reference unsafe() const { return value; }

 private:
  mutable Mutex mtx;
  value_type value;
};

template<typename T, typename Mutex>
template<typename Safe<T, Mutex>::AccessType AccessPolicy>
class Safe<T, Mutex>::Access final {
  using reference_type = std::conditional_t<AccessPolicy == Safe<T>::AccessType::ReadWrite, reference, const_reference>;
  using pointer_type = std::conditional_t<AccessPolicy == Safe<T>::AccessType::ReadWrite, pointer, const_pointer>;

 public:
  Access(reference_type value, Mutex &mtx) : value(value), lck(mtx) {}
  Access(Safe<T, Mutex> &safe) : Access(safe.value, safe.mtx) {}
  Access(const Access &other) = delete;
  Access(Access &&other) = delete;
  Access &operator=(const Access &other) = delete;
  Access &operator=(Access &&other) = delete;

  reference_type operator*() noexcept { return value; }
  const_reference operator*() const noexcept { return value; }
  pointer_type operator->() noexcept { return &value; }
  const_pointer operator->() const noexcept { return &value; }

 private:
  reference_type value;
  std::unique_lock<Mutex> lck;
};

template<typename T, typename Mutex = std::mutex>
using ReadOnlyAccess = typename Safe<T, Mutex>::template Access<Safe<T, Mutex>::AccessType::ReadOnly>;

template<typename T, typename Mutex = std::mutex>
using WriteAccess = typename Safe<T, Mutex>::template Access<Safe<T, Mutex>::AccessType::ReadWrite>;
}// namespace pf
#endif// PF_COMMON_SAFE_H
