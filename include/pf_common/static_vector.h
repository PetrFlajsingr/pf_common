//
// Created by xflajs00 on 11.11.2022.
//

#ifndef PF_COMMON_STATIC_VECTOR_H
#define PF_COMMON_STATIC_VECTOR_H

#include <cassert>
#include <iterator>
#include <memory>
#include <new>
#include <type_traits>

#ifndef PF_STATIC_VECTOR_ASSERT
// FIXME: better impl
#define PF_STATIC_VECTOR_ASSERT(b, msg)                                                                                \
  do { assert((b) && msg); } while (0)
#endif

#ifndef PF_STATIC_VECTOR_ENABLE_BOUND_CHECKS
#define PF_STATIC_VECTOR_ENABLE_BOUND_CHECKS false
#else
static_assert(std::same_as<bool, decltype(PF_STATIC_VECTOR_ENABLE_BOUND_CHECKS)>, "Bool required");
#endif

#ifndef PF_STATIC_VECTOR_DEBUG_T_PTR_MEMBER_ENABLE
#define PF_STATIC_VECTOR_DEBUG_T_PTR_MEMBER_ENABLE false
#else
static_assert(std::same_as<bool, decltype(PF_STATIC_VECTOR_DEBUG_T_PTR_MEMBER_ENABLE)>, "Bool required");
#endif

#if PF_STATIC_VECTOR_ENABLE_BOUND_CHECKS
#define PF_STATIC_VECTOR_ASSERT_BOUNDS(index)                                                                          \
  PF_STATIC_VECTOR_ASSERT(index < size(), "Attempting to access uninitialized memory")
#define PF_STATIC_VECTOR_ASSERT_ITERATOR_VALID(iter)                                                                   \
  PF_STATIC_VECTOR_ASSERT(begin() <= iter && iter <= end(), "Iterator out of bounds")
#else
#define PF_STATIC_VECTOR_ASSERT_BOUNDS(index)
#define PF_STATIC_VECTOR_ASSERT_ITERATOR_VALID(iter)
#endif

namespace pf {

template<typename T, size_t N>
class static_vector {
 public:
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = std::size_t;
  using difference_type = std::make_signed_t<size_type>;
  using iterator = pointer;
  using const_iterator = const_pointer;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  constexpr static_vector() noexcept : end_{data()} {}
  constexpr explicit static_vector(size_type n)
    requires(std::is_default_constructible_v<value_type>)
      : end_{data() + n} {
    PF_STATIC_VECTOR_ASSERT(n < max_size(), "Attempting to allocate more memory than available");
    std::ranges::uninitialized_default_construct(*this);
    init_ptrs();
  }
  constexpr static_vector(size_type n, const value_type &value)
    requires(std::is_copy_constructible_v<value_type>)
      : end_{data() + n} {
    PF_STATIC_VECTOR_ASSERT(n < max_size(), "Attempting to allocate more memory than available");
    std::ranges::uninitialized_fill(*this, value);
    init_ptrs();
  }
  template<std::forward_iterator InputIterator, std::sentinel_for<InputIterator> Sentinel>
  constexpr static_vector(InputIterator first, Sentinel last) {
    const auto src_size = std::ranges::distance(first, last);
    PF_STATIC_VECTOR_ASSERT(src_size < max_size(), "Attempting to allocate more memory than available");
    end_ = data() + src_size;
    std::ranges::uninitialized_copy(first, last, begin(), end());
    init_ptrs();
  }
  constexpr static_vector(const static_vector &other) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
    requires(std::is_copy_constructible_v<value_type>)
      : end_{data() + other.size()} {
    std::ranges::uninitialized_copy(other, *this);
    init_ptrs();
  }
  constexpr static_vector(static_vector &&other) noexcept(std::is_nothrow_move_constructible_v<value_type>)
    requires (std::is_move_constructible_v<T>)
      : end_{data() + other.size()} {
    std::ranges::uninitialized_move(std::move(other), *this);
    init_ptrs();
  }
  constexpr static_vector(std::initializer_list<value_type> il) : end_{data() + il.size()} {
    PF_STATIC_VECTOR_ASSERT(il.size() < max_size(), "Attempting to allocate more memory than available");
    std::ranges::uninitialized_copy(il.begin(), il.end(), begin(), end());
    init_ptrs();
  }

  [[nodiscard]] friend constexpr std::weak_ordering operator<=>(const static_vector<T, N> &lhs, const static_vector<T, N> &rhs) {
    if constexpr (std::three_way_comparable<T>) {
      return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    } else {
      for (auto b1 = lhs.begin(), b2 = rhs.begin(); b1 != lhs.end(); ++b1, ++b2) {
        if (*b1 < *b2) { return std::weak_ordering::less; }
        if (*b1 > *b2) { return std::weak_ordering::greater; }
      }
      if (lhs.size() < rhs.size()) { return std::weak_ordering::less; }
      if (lhs.size() > rhs.size()) { return std::weak_ordering::greater; }
      return std::weak_ordering::equivalent;
    }
  }

  [[nodiscard]] friend bool operator==(const static_vector &lhs, const static_vector &rhs) { return lhs <=> rhs == std::weak_ordering::equivalent; }
  [[nodiscard]] friend bool operator!=(const static_vector &lhs, const static_vector &rhs) { return !(rhs == lhs); }

  [[nodiscard]] friend bool operator<(const static_vector &lhs, const static_vector &rhs) { return lhs <=> rhs == std::weak_ordering::less; }
  [[nodiscard]] friend bool operator>(const static_vector &lhs, const static_vector &rhs) { return lhs <=> rhs == std::weak_ordering::greater; }
  [[nodiscard]] friend bool operator<=(const static_vector &lhs, const static_vector &rhs) { return !(rhs < lhs); }
  [[nodiscard]] friend bool operator>=(const static_vector &lhs, const static_vector &rhs) { return !(lhs < rhs); }

  constexpr static_vector &
  operator=(const static_vector &other) noexcept(std::is_nothrow_copy_assignable_v<value_type>) {
    if (this == &other) { return *this; }
    auto this_iter = begin();
    auto other_iter = other.begin();
    for (; this_iter != end() && other_iter != other.end(); ++this_iter, ++other_iter) { *this_iter = *other_iter; }
    for (; other_iter != other.end(); ++this_iter, ++other_iter) { new (this_iter) T{*other_iter}; }
    if (this_iter < end_) { destroy_elements(this_iter, end_); }
    end_ = this_iter;
    return *this;
  }
  constexpr static_vector &operator=(static_vector &&other) noexcept(std::is_nothrow_move_assignable_v<value_type>) {
    if (this == &other) { return *this; }
    auto this_iter = begin();
    auto other_iter = other.begin();
    for (; this_iter != end() && other_iter != other.end(); ++this_iter, ++other_iter) {
      *this_iter = std::move(*other_iter);
    }
    for (; other_iter != other.end(); ++this_iter, ++other_iter) { new (this_iter) T{std::move(*other_iter)}; }
    if (this_iter < end_) { destroy_elements(this_iter, end_); }
    end_ = this_iter;
    return *this;
  }
  template<std::input_iterator InputIterator, std::sentinel_for<InputIterator> Sentinel>
  constexpr void assign(InputIterator first, Sentinel last) {
    [[maybe_unused]] const auto src_size = std::ranges::distance(first, last);
    PF_STATIC_VECTOR_ASSERT(src_size < max_size(), "Attempting to allocate more memory than available");
    auto this_iter = begin();
    auto other_iter = first;
    for (; this_iter != end() && other_iter != last; ++this_iter, ++other_iter) { *this_iter = *other_iter; }
    for (; other_iter != last; ++this_iter, ++other_iter) { new (this_iter) T{std::move(*other_iter)}; }
    if (this_iter != end_) { destroy_elements(this_iter, end_); }
    end_ = this_iter;
  }
  constexpr void assign(size_type n, const value_type &u) {
    PF_STATIC_VECTOR_ASSERT(n < max_size(), "Attempting to allocate more memory than available");
    auto this_iter = begin();
    auto index = 0;
    for (; this_iter != end() && index < n; ++this_iter, ++index) { *this_iter = u; }
    for (; index < n; ++this_iter, ++index) { new (this_iter) T{u}; }
    if (this_iter != end_) { destroy_elements(this_iter, end_); }
    end_ = this_iter;
  }
  constexpr void assign(std::initializer_list<value_type> il) { assign(std::ranges::begin(il), std::ranges::end(il)); }

  constexpr ~static_vector() { clear(); }

  [[nodiscard]] constexpr iterator begin() noexcept { return data(); }
  [[nodiscard]] constexpr const_iterator begin() const noexcept { return data(); }
  [[nodiscard]] constexpr iterator end() noexcept { return end_; }
  [[nodiscard]] constexpr const_iterator end() const noexcept { return end_; }
  [[nodiscard]] constexpr reverse_iterator rbegin() noexcept { return end_ - 1; }
  [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept { return end_ - 1; }
  [[nodiscard]] constexpr reverse_iterator rend() noexcept { return data() - 1; }
  [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept { return data() - 1; }
  [[nodiscard]] constexpr const_iterator cbegin() noexcept { return data(); }
  [[nodiscard]] constexpr const_iterator cend() const noexcept { return end_; }
  [[nodiscard]] constexpr const_reverse_iterator crbegin() noexcept { return end_ - 1; }
  [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept { return data() - 1; }

  [[nodiscard]] constexpr bool empty() const noexcept { return data() == end_; }
  [[nodiscard]] constexpr size_type size() const noexcept { return std::ranges::distance(data(), end_); }
  [[nodiscard]] static constexpr size_type max_size() noexcept { return N; }
  [[nodiscard]] static constexpr size_type capacity() noexcept { return N; }
  constexpr void resize(size_type sz)
    requires(std::is_default_constructible_v<T>)
  {
    PF_STATIC_VECTOR_ASSERT(sz < max_size(), "Attempting to allocate more memory than available");
    if (sz < size()) {
      destroy_elements(begin() + sz, end());
    } else {
      std::uninitialized_default_construct(data() + size(), data() + sz);
    }
    end_ = data() + sz;
  }
  constexpr void resize(size_type sz, const value_type &c) {
    PF_STATIC_VECTOR_ASSERT(sz < max_size(), "Attempting to allocate more memory than available");
    if (sz < size()) {
      destroy_elements(begin() + sz, end());
    } else {
      std::uninitialized_fill(data() + size(), data() + sz, c);
    }
    end_ = data() + sz;
  }

  [[nodiscard]] constexpr reference operator[](size_type n) {
    PF_STATIC_VECTOR_ASSERT_BOUNDS(n);
    return data()[n];
  }
  [[nodiscard]] constexpr const_reference operator[](size_type n) const {
    PF_STATIC_VECTOR_ASSERT_BOUNDS(n);
    return data()[n];
  }
  [[nodiscard]] constexpr reference front() {
    PF_STATIC_VECTOR_ASSERT_BOUNDS(0);
    return data()[0];
  }
  [[nodiscard]] constexpr const_reference front() const {
    PF_STATIC_VECTOR_ASSERT_BOUNDS(0);
    return data()[0];
  }
  [[nodiscard]] constexpr reference back() {
    PF_STATIC_VECTOR_ASSERT_BOUNDS(size() - 1);
    return data()[size() - 1];
  }
  [[nodiscard]] constexpr const_reference back() const {
    PF_STATIC_VECTOR_ASSERT_BOUNDS(size() - 1);
    return data()[size() - 1];
  }
  [[nodiscard]] constexpr pointer data() noexcept { return std::launder(reinterpret_cast<value_type *>(storage)); }
  [[nodiscard]] constexpr const_pointer data() const noexcept {
    return std::launder(reinterpret_cast<const value_type *>(storage));
  }

  constexpr iterator insert(const_iterator position, const value_type &x) {
    PF_STATIC_VECTOR_ASSERT(size() + 1 < max_size(), "Attempting to allocate more memory than available");
    PF_STATIC_VECTOR_ASSERT_ITERATOR_VALID(position);

    const auto target = insert_empty(position, 1);
    *target = x;
    return target;
  }
  constexpr iterator insert(const_iterator position, value_type &&x) {
    PF_STATIC_VECTOR_ASSERT(size() + 1 < max_size(), "Attempting to allocate more memory than available");
    PF_STATIC_VECTOR_ASSERT_ITERATOR_VALID(position);

    const auto target = insert_empty(position, 1);
    *target = std::move(x);
    return target;
  }
  constexpr iterator insert(const_iterator position, size_type n, const value_type &x) {
    PF_STATIC_VECTOR_ASSERT(size() + n < max_size(), "Attempting to allocate more memory than available");
    PF_STATIC_VECTOR_ASSERT_ITERATOR_VALID(position);

    const auto target = insert_empty(position, n);
    for (size_type i = 0; i < n; ++i) { *(target + i) = x; }
    return target;
  }
  template<std::input_iterator InputIterator, std::sentinel_for<InputIterator> Sentinel>
  constexpr iterator insert(const_iterator position, InputIterator first, Sentinel last) {
    const auto count = std::ranges::distance(first, last);
    PF_STATIC_VECTOR_ASSERT(size() + count < max_size(), "Attempting to allocate more memory than available");
    PF_STATIC_VECTOR_ASSERT_ITERATOR_VALID(position);

    const auto result = insert_empty(position, count);
    for (auto target = result; first != last; ++first, ++target) { *target = *first; }
    return result;
  }
  constexpr iterator insert(const_iterator position, std::initializer_list<value_type> il) {
    return insert(position, std::ranges::begin(il), std::ranges::end(il));
  }

  template<typename... Args>
  constexpr iterator emplace(const_iterator position, Args &&...args) {
    PF_STATIC_VECTOR_ASSERT(size() + 1 < max_size(), "Attempting to allocate more memory than available");
    PF_STATIC_VECTOR_ASSERT_ITERATOR_VALID(position);

    const auto target = insert_empty(position, 1);
    return new (target) T{std::forward<Args>(args)...};
  }
  template<typename... Args>
  constexpr reference emplace_back(Args &&...args) {
    PF_STATIC_VECTOR_ASSERT(size() + 1 < max_size(), "Attempting to allocate more memory than available");

    auto newElement = new (&storage[size() * sizeof(T)]) T{std::forward<Args>(args)...};
    ++end_;
    return *newElement;
  }
  constexpr void push_back(const value_type &x) {
    PF_STATIC_VECTOR_ASSERT(size() + 1 < max_size(), "Attempting to allocate more memory than available");

    emplace_back(x);
  }
  constexpr void push_back(value_type &&x) {
    PF_STATIC_VECTOR_ASSERT(size() + 1 < max_size(), "Attempting to allocate more memory than available");

    emplace_back(std::move(x));
  }

  constexpr void pop_back() {
    if (empty()) { return; }
    destroy_elements(end() - 1, end());
    --end_;
  }
  constexpr iterator erase(const_iterator position) {
    PF_STATIC_VECTOR_ASSERT_ITERATOR_VALID(position);

    const auto index = std::ranges::distance(begin(), position);
    const auto last_src = end_ - 1;
    for (auto dst = begin() + index; dst != last_src; ++dst) { *dst = std::move(*(dst + 1)); }
    last_src->~T();
    --end_;
    return begin() + index;
  }
  constexpr iterator erase(const_iterator first, const_iterator last) {
    PF_STATIC_VECTOR_ASSERT_ITERATOR_VALID(first);
    PF_STATIC_VECTOR_ASSERT_ITERATOR_VALID(last);

    const auto index = std::ranges::distance(begin(), first);
    const auto remove_count = std::ranges::distance(first, last);
    for (auto dst = begin() + index; dst + remove_count != end_; ++dst) { *dst = std::move(*(dst + remove_count)); }
    for (auto to_destroy = end_ - remove_count; to_destroy != end_; ++to_destroy) { to_destroy->~T(); }
    end_ -= remove_count;
    return begin() + index;
  }

  constexpr void clear() noexcept {
    destroy_elements(begin(), end());
    end_ = data();
  }

  constexpr void swap(static_vector &x) noexcept(std::is_nothrow_swappable_v<value_type>) {
    for (auto b1 = begin(), b2 = x.begin(); b1 != end(); ++b1, ++b2) { std::swap(*b1, *b2); }
  }

 private:
  constexpr void destroy_elements(iterator first, iterator last) {
    for (; first != last; ++first) { first->~T(); }
  }
  constexpr iterator insert_empty(const_iterator position, size_type count) {
    const auto new_end = end_ + count;
    const auto index = std::ranges::distance(begin(), position);
    const auto result_iter = begin() + index;
    const auto last_source = result_iter + count - 1;
    for (auto target = new_end - 1; target != last_source; --target) {
      const auto src = target - count;
      if (target >= end_) {
        new (target) T{std::move(*src)};
      } else {
        *target = std::move(*src);
      }
    }
    end_ = new_end;
    return result_iter;
  }

  alignas(T) std::byte storage[sizeof(T) * N]{};
  pointer end_;

#if PF_STATIC_VECTOR_DEBUG_T_PTR_MEMBER_ENABLE
  std::array<T*, N> ptrs;

  void init_ptrs() {
    for (std::size_t i = 0ull; i < N; ++i) {
      ptrs[i] = data() + i;
    }
  }
#else
  void init_ptrs() {}
#endif
};

template<typename T, size_t N>
constexpr void swap(static_vector<T, N> &x, static_vector<T, N> &y) noexcept(noexcept(x.swap(y))) {
  x.swap(y);
}

}  // namespace pf

#endif  //PF_COMMON_STATIC_VECTOR_H
