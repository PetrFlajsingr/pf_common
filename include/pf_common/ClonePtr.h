//
// Created by Petr on 22.04.2021.
//

#ifndef PF_COMMON_INCLUDE_PF_COMMON_CLONEPTR_H
#define PF_COMMON_INCLUDE_PF_COMMON_CLONEPTR_H

#include <memory>

namespace pf {

// TODO: deleter
// TODO: type requirements
template <typename T, typename Deleter = std::default_delete<T>> class ClonePtr {
  template <typename U, typename OtherDeleter> friend class ClonePtr;

 public:
  using element_type = std::remove_extent_t<T>;
  explicit ClonePtr(T *ptr) : ptr(ptr) {}
  ~ClonePtr() {
    delete ptr;
  }
  template <typename U> ClonePtr(const ClonePtr<U> &other) {
    ptr = new U(*other.ptr);
  }
  template <typename U> ClonePtr(ClonePtr<U> &&other) noexcept {
    ptr = other.ptr;
    other.ptr = nullptr;
  }

  template <typename U> ClonePtr &operator=(const ClonePtr<U> &other) {
    if (this == &other) {
      return *this;
    }
    delete ptr;
    ptr = new U(*other.ptr);
    return *this;
  }
  template <typename U> ClonePtr &operator=(ClonePtr<U> &&other) noexcept {
    if (this == &other) {
      return *this;
    }
    delete ptr;
    ptr = other.ptr;
    other.ptr = nullptr;
    return *this;
  }

  void reset() {
    delete ptr;
    ptr = nullptr;
  }

  T *get() { return ptr; }

  T &operator*() { return *ptr; }
  T *operator->() { return ptr; }

  explicit operator bool() { return ptr != nullptr; }

 private:
  T *ptr;
};

template <typename T, typename U, typename Deleter> ClonePtr<T, Deleter> dynamic_pointer_cast(const ClonePtr<U, Deleter> &r) noexcept {
if (auto p = dynamic_cast<typename ClonePtr<T, Deleter>::element_type *>(r.get())) {
return ClonePtr<T, Deleter>(r);
} else {
return ClonePtr<T, Deleter>(nullptr);
}
}

template <typename T, typename... Args> ClonePtr<T> make_clone(Args &&... args) { return ClonePtr<T>(new T(args...)); }


}

#endif//PF_COMMON_INCLUDE_PF_COMMON_CLONEPTR_H
