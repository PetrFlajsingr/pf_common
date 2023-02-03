//
// Created by Petr.Flajsingr on 2023-02-03.
//

#ifndef PF_COMMON_SMALLVECTORALLOCATOR_H
#define PF_COMMON_SMALLVECTORALLOCATOR_H

#include <cstddef>
#include <memory>
#include <pf_common/concepts/Allocator.h>

namespace pf {

template<typename T, std::size_t N, Allocator SecondaryAllocator = std::allocator<T>, typename PreRebindT = T>
class SmallVectorAllocator {
  template<typename U>
  using SecondaryAllocatorRebind = typename std::allocator_traits<SecondaryAllocator>::template rebind_alloc<U>;

 public:
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using secondary_allocator = SecondaryAllocator;

  using propagate_on_container_move_assignment = std::false_type;
  using propagate_on_container_swap = std::false_type;
  using is_always_equal = std::false_type;
  template<typename U>
  struct rebind {
    using other = SmallVectorAllocator<U, N, SecondaryAllocatorRebind<U>, PreRebindT>;
  };

  constexpr SmallVectorAllocator(SecondaryAllocator allocator = {}) noexcept : allocator{std::move(allocator)} {}

  template<typename U>
  constexpr SmallVectorAllocator(const SmallVectorAllocator<U, N, SecondaryAllocatorRebind<U>, PreRebindT> &other) noexcept
      : allocator{other.get_secondary_allocator()} {}

  constexpr SmallVectorAllocator(const SmallVectorAllocator &other) noexcept
      : stackStorageInUse{other.stackStorageInUse}, allocator{other.allocator} {}

  constexpr SmallVectorAllocator(SmallVectorAllocator &&other) noexcept
      : stackStorageInUse{other.stackStorageInUse}, allocator{std::move(other.allocator)} {}

  constexpr SmallVectorAllocator &operator=(const SmallVectorAllocator &other) noexcept {
    allocator = other.allocator;
    stackStorageInUse = other.stackStorageInUse;
    return *this;
  }
  constexpr SmallVectorAllocator &operator=(SmallVectorAllocator &&other) noexcept {
    allocator = std::move(other.allocator);
    stackStorageInUse = other.stackStorageInUse;
    return *this;
  }
  [[nodiscard]] friend constexpr bool operator==(const SmallVectorAllocator &lhs, const SmallVectorAllocator &rhs) {
    return !lhs.stackStorageInUse && !rhs.stackStorageInUse;
  }
  [[nodiscard]] friend constexpr bool operator!=(const SmallVectorAllocator &lhs, const SmallVectorAllocator &rhs) { return !(lhs == rhs); }

  [[nodiscard]] constexpr pointer allocate(size_type n) noexcept(noexcept(allocator.allocate(n))) {
    if constexpr (std::same_as<PreRebindT, T>) {
      if (n <= N) {
        stackStorageInUse = true;
        return std::launder(reinterpret_cast<value_type *>(storage));
      }
    }
    stackStorageInUse = false;
    return allocator.allocate(n);
  }

  constexpr void deallocate(T *ptr, size_type n) noexcept(noexcept(allocator.deallocate(ptr, n))) {
    // no need to deallocate stack storage
    if (ptr == std::launder(reinterpret_cast<value_type *>(storage))) {
      stackStorageInUse = false;
      return;
    }
    allocator.deallocate(ptr, n);
  }

  [[nodiscard]] constexpr secondary_allocator get_secondary_allocator() const { return allocator; }

 private:
  alignas(T) std::byte storage[sizeof(T) * N];
  bool stackStorageInUse = true;

  secondary_allocator allocator;
};
}// namespace pf

#endif//PF_COMMON_SMALLVECTORALLOCATOR_H
