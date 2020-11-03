//
// Created by Petr on 15.09.2020.
//

#ifndef DESIGN_PATTERNS_OBJECT_POOL_H
#define DESIGN_PATTERNS_OBJECT_POOL_H

#include "../concepts.h"
#include <algorithm>
#include <concepts>
#include <exception>
#include <iostream>
#include <list>
#include <mutex>
#include <experimental/memory>

namespace pf {

enum class PoolAllocStrategy {
  Preallocate,
  IncreaseBy2x,
  OnDemand
};


namespace details {
template <PoolAllocStrategy Strategy>
struct DefaultPoolAllocatorData {};
template <>
struct DefaultPoolAllocatorData<PoolAllocStrategy::IncreaseBy2x> {
  std::size_t capacity_ = 0;
};
template <>
struct DefaultPoolAllocatorData<PoolAllocStrategy::OnDemand> {
  std::size_t capacity_ = 0;
};

template<typename T, std::size_t PoolSize, PoolAllocStrategy Strategy>
struct DefaultPoolAllocator {
  DefaultPoolAllocatorData<Strategy> data;

  explicit DefaultPoolAllocator(std::invocable auto &&generator,
                                  std::list<std::unique_ptr<T>> &available,
                                  std::list<std::unique_ptr<T>> &inUse) : generator(generator) {
    if constexpr (Strategy == PoolAllocStrategy::Preallocate) {
      alloc_n(available, inUse, PoolSize);
    }
  }
  void on_lease(std::list<std::unique_ptr<T>> &available, std::list<std::unique_ptr<T>> &inUse) {
    if constexpr (Strategy == PoolAllocStrategy::IncreaseBy2x) {
      if (data.capacity_ == inUse.size() && data.capacity_ < PoolSize) {
        const auto n_to_alloc = std::max(std::min(data.capacity_ * 2, PoolSize - data.capacity_), 1ull);
        alloc_n(available, inUse,n_to_alloc);
        data.capacity_ += n_to_alloc;
      }
    } else if constexpr (Strategy == PoolAllocStrategy::OnDemand) {
      if (data.capacity_ == inUse.size() && data.capacity_ < PoolSize) {
        alloc_n(available, inUse,1);
        ++data.capacity_;
      }
    }
  }
  void on_release(std::list<std::unique_ptr<T>> &available, std::list<std::unique_ptr<T>> &inUse) {
  }

  std::size_t capacity() {
    return data.capacity_;
  }

  void shrink(std::list<std::unique_ptr<T>> &available, std::list<std::unique_ptr<T>> &inUse) {
    const auto remove_cnt = inUse.size();
    inUse.clear();
    data.capacity_ -= remove_cnt;
  }

  void alloc_n(std::list<std::unique_ptr<T>> &available, std::list<std::unique_ptr<T>> &inUse, std::size_t n) {
    std::generate_n(std::back_inserter(available), n, [&] { return std::make_unique<T>(generator()); });
    std::cout << "allocating " << n << std::endl;
  }

  std::function<T()> generator;
};
}// namespace details

template<typename T, std::size_t PoolSize, PoolAllocStrategy Strategy = PoolAllocStrategy::Preallocate>
class object_pool {
  using pool_allocator = details::DefaultPoolAllocator<T, PoolSize, Strategy>;
 public:
  using size_type = std::size_t;
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using pointer = std::experimental::observer_ptr<T>;
  using const_pointer = std::experimental::observer_ptr<const T>;

  object_pool() requires std::default_initializable<T> : allocator([] {return T();}, available_, inUse) {}

  explicit object_pool(std::invocable auto &&generator) : allocator(generator, available_, inUse) {
  }

  [[nodiscard]] pointer lease() {
    std::unique_lock lock{mutex};
    allocator.on_lease(available_, inUse);
    if (available_.empty()) {
      throw std::runtime_error{"Pool has no available objects."};
    }
    const auto first_available_iter = available_.begin();
    auto &ref = inUse.emplace_front(std::move(*first_available_iter));
    available_.erase(first_available_iter);
    return std::experimental::make_observer(ref.get());
  }

  void release(pointer object) {
    std::unique_lock lock{mutex};
    if (auto iter = std::find_if(inUse.begin(), inUse.end(), [&object](const auto &obj) {
          return obj.get() == object.get();
        });
        iter != inUse.end()) {
      auto &ref = available_.emplace_back(std::move(*iter));
      inUse.erase(iter);
      allocator.on_release(available_, inUse);
    }
  }

  [[nodiscard]] size_type capacity() const{
    if constexpr (Strategy == PoolAllocStrategy::Preallocate) {
      return PoolSize;
    } else {
      return allocator.data.capacity();
    }
  }

  void shrink_to_fit() requires (Strategy != PoolAllocStrategy::Preallocate) {
    allocator.shrink(available_, inUse);
  }

  [[nodiscard]] size_type used() const {
    return inUse.size();
  }

  [[nodiscard]] size_type available() const {
    return available_.size();
  }

 private:
  std::mutex mutex;
  std::list<std::unique_ptr<T>> inUse;
  std::list<std::unique_ptr<T>> available_;
  pool_allocator allocator;
};
}// namespace pf
#endif//DESIGN_PATTERNS_OBJECT_POOL_H
