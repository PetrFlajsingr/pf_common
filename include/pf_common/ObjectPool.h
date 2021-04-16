//
// Created by Petr on 15.09.2020.
//

#ifndef PF_COMMON_OBJECT_POOL_H
#define PF_COMMON_OBJECT_POOL_H

#include <algorithm>
#include <concepts>
#include <exception>
#include <experimental/memory>
#include <iostream>
#include <list>
#include <mutex>

namespace pf {

/**
 * Allocation strategy for ObjectPool.
 */
enum class PoolAllocStrategy {
  Preallocate,  /**< Allocate whole pool on creation. */
  IncreaseBy2x, /**< Allocate 2x the current capacity each time there's not enough objects. */
  OnDemand      /**< Allocate capacity + 1 each time there's not enough objects. */
};

namespace details {
template<PoolAllocStrategy Strategy>
struct DefaultPoolAllocatorData {};
template<>
struct DefaultPoolAllocatorData<PoolAllocStrategy::IncreaseBy2x> {
  std::size_t capacity_ = 0;
};
template<>
struct DefaultPoolAllocatorData<PoolAllocStrategy::OnDemand> {
  std::size_t capacity_ = 0;
};

template<typename T, std::size_t PoolSize, PoolAllocStrategy Strategy>
struct DefaultPoolAllocator {
  DefaultPoolAllocatorData<Strategy> data;

  explicit DefaultPoolAllocator(std::invocable auto &&generator, std::list<std::unique_ptr<T>> &available,
                                std::list<std::unique_ptr<T>> &inUse)
      : generator(generator) {
    if constexpr (Strategy == PoolAllocStrategy::Preallocate) { alloc_n(available, inUse, PoolSize); }
  }
  void on_lease(std::list<std::unique_ptr<T>> &available, std::list<std::unique_ptr<T>> &inUse) {
    if constexpr (Strategy == PoolAllocStrategy::IncreaseBy2x) {
      if (data.capacity_ == inUse.size() && data.capacity_ < PoolSize) {
        const auto n_to_alloc = std::max(std::min(data.capacity_ * 2, PoolSize - data.capacity_), 1ull);
        alloc_n(available, inUse, n_to_alloc);
        data.capacity_ += n_to_alloc;
      }
    } else if constexpr (Strategy == PoolAllocStrategy::OnDemand) {
      if (data.capacity_ == inUse.size() && data.capacity_ < PoolSize) {
        alloc_n(available, inUse, 1);
        ++data.capacity_;
      }
    }
  }
  void on_release(std::list<std::unique_ptr<T>> &available, std::list<std::unique_ptr<T>> &inUse) {}

  std::size_t capacity() { return data.capacity_; }

  void shrink(std::list<std::unique_ptr<T>> &available, std::list<std::unique_ptr<T>> &inUse) {
    const auto remove_cnt = inUse.size();
    inUse.clear();
    data.capacity_ -= remove_cnt;
  }

  void alloc_n(std::list<std::unique_ptr<T>> &available, std::list<std::unique_ptr<T>> &inUse, std::size_t n) {
    std::generate_n(std::back_inserter(available), n, [&] { return std::make_unique<T>(generator()); });
  }

  std::function<T()> generator;
};
}// namespace details

/**
 * @brief Statically sized object pool to avoid multiple allocations and deallocations of objects.
 * Thread-safe.
 * @tparam T type of the stored objects
 * @tparam PoolSize maximum size of the pool
 * @tparam Strategy allocation strategy
 */
template<typename T, std::size_t PoolSize, PoolAllocStrategy Strategy = PoolAllocStrategy::Preallocate>
class ObjectPool {
  using pool_allocator = details::DefaultPoolAllocator<T, PoolSize, Strategy>;

 public:
  using size_type = std::size_t;
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using pointer = std::experimental::observer_ptr<T>;
  using const_pointer = std::experimental::observer_ptr<const T>;

  /**
   * Construct ObjectPool. Only accessible if T is default initializable.
   */
  ObjectPool() requires std::default_initializable<T> : allocator([] { return T(); }, available_, inUse) {}

  /**
   * Construct ObjectPool with a generator to create stored instances.
   * @param generator generator for T
   */
  explicit ObjectPool(std::invocable auto &&generator) requires(std::same_as<T, decltype(generator())>)
      : allocator(std::forward<decltype(generator)>(generator), available_, inUse) {}

  /**
   * Get an available object from a pool.
   * Objects has to be returned via release method for it to be available again.
   * @return pointer to the leased object
   * @throws std::runtime_error when there are no more objects available
   */
  [[nodiscard]] pointer lease() {
    std::unique_lock lock{mutex};
    allocator.on_lease(available_, inUse);
    if (available_.empty()) { throw std::runtime_error{"Pool has no available objects."}; }
    const auto first_available_iter = available_.begin();
    auto &ref = inUse.emplace_front(std::move(*first_available_iter));
    available_.erase(first_available_iter);
    return std::experimental::make_observer(ref.get());
  }

  /**
   * Return an object back to the pool for it to be made available.
   * @param object object to return
   */
  void release(pointer object) {
    std::unique_lock lock{mutex};
    if (auto iter =
            std::find_if(inUse.begin(), inUse.end(), [&object](const auto &obj) { return obj.get() == object.get(); });
        iter != inUse.end()) {
      auto &ref = available_.emplace_back(std::move(*iter));
      inUse.erase(iter);
      allocator.on_release(available_, inUse);
    }
  }

  /**
   * Get current capacity of the pool.
   * @return capacity of the pool
   */
  [[nodiscard]] constexpr size_type capacity() const {
    if constexpr (Strategy == PoolAllocStrategy::Preallocate) {
      return PoolSize;
    } else {
      return allocator.data.capacity();
    }
  }

  /**
   * Deallocate objects which are currently not leased.
   */
  void shrink_to_fit() requires(Strategy != PoolAllocStrategy::Preallocate) { allocator.shrink(available_, inUse); }

  /**
   * Get count of leased objects.
   * @return count of leased objects
   */
  [[nodiscard]] size_type used() const { return inUse.size(); }

  /**
   * Get count of available objects.
   * @return count of available objects
   */
  [[nodiscard]] size_type available() const { return available_.size(); }

 private:
  std::mutex mutex;
  std::list<std::unique_ptr<T>> inUse;
  std::list<std::unique_ptr<T>> available_;
  pool_allocator allocator;
};
}// namespace pf
#endif//PF_COMMON_OBJECT_POOL_H
