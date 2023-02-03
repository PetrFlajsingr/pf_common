//
// Created by Petr.Flajsingr on 2023-02-03.
//

#ifndef PF_COMMON_SMALLVECTOR_H
#define PF_COMMON_SMALLVECTOR_H

#include <pf_common/allocators/SmallVectorAllocator.h>
#include <vector>

namespace pf {

template<typename T, std::size_t N, typename SecondaryAllocator = std::allocator<T>>
class SmallVector : public std::vector<T, SmallVectorAllocator<T, N, SecondaryAllocator>> {
  using Base = std::vector<T, SmallVectorAllocator<T, N, SecondaryAllocator>>;

 public:
  constexpr SmallVector() noexcept { Base::reserve(N); }
  constexpr SmallVector(const SmallVector &) = default;
  constexpr SmallVector &operator=(const SmallVector &) = default;
  constexpr SmallVector(SmallVector &&other) noexcept(std::is_nothrow_move_constructible_v<T>) {
    if (other.size() <= N) { Base::reserve(N); }
    Base::operator=(std::move(other));
  }
  constexpr SmallVector &operator=(SmallVector &&other) noexcept(std::is_nothrow_move_constructible_v<T>) {
    if (other.size() <= N) { Base::reserve(N); }
    Base::operator=(std::move(other));
    return *this;
  }

  constexpr explicit SmallVector(size_t count) : SmallVector() { Base::resize(count); }
  constexpr SmallVector(size_t count, const T &value) : SmallVector() { Base::assign(count, value); }
  template<typename InputIt>
  constexpr SmallVector(InputIt first, InputIt last) : SmallVector() {
    Base::assign(first, last);
  }
  constexpr SmallVector(std::initializer_list<T> init) : SmallVector() { Base::assign(init); }
  constexpr friend void swap(SmallVector &a, SmallVector &b) noexcept {
    using std::swap;
    swap(static_cast<Base &>(a), static_cast<Base &>(b));
  }
};

}// namespace pf

#endif//PF_COMMON_SMALLVECTOR_H
