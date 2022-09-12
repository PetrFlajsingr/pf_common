/**
 * @name View2D.h
 * @brief 2D view for data stored contiguously in memory
 * @author Petr Flajšingr, xflajs00
 * @date 31.03.2021
 */

#ifndef PF_COMMON_VIEWS_VIEW2D_H
#define PF_COMMON_VIEWS_VIEW2D_H

#include <cmath>
#include <memory>
#include <ranges>
#include <span>
#include <type_traits>

namespace pf {
/**
 * 2D view for data stored contiguously in memory
 * @tparam IsConst read-only view if true, read-write otherwise
 */
template<std::ranges::contiguous_range R, bool IsConst>
class View2D {
 public:
  using range_storage = std::conditional_t<IsConst, const R *, R *>;
  using range_reference = std::conditional_t<IsConst, const R &, R &>;
  using value_type = std::ranges::range_value_t<R>;
  using reference = std::ranges::range_reference_t<R>;
  using const_reference = const value_type &;

  View2D() : range(nullptr) {}
  /**
   * @param width row length
   */
  View2D(range_reference range, size_t width) : range(&range), width(width) {}

  View2D(const View2D &other) : range(other.range), width(other.width) {}
  View2D &operator=(const View2D &other) {
    range = other.range;
    width = other.width;
    return *this;
  }
  View2D(const View2D<R, true> &other)
    requires(!IsConst)
  : range(other.range), width(other.width) {}
  View2D &operator=(const View2D<R, true> &other)
    requires(!IsConst)
  {
    range = other.range;
    width = other.width;
    return *this;
  }

  /**
   * Accessor for data in column.
   * @tparam IsConstAccessor read-only if true, read-write otherwise
   */
  template<bool IsConstAccessor>
  class RowAccessor {
   public:
    RowAccessor(range_storage range, size_t col, size_t width) : range(range), column(col), width(width) {}
    [[nodiscard]] reference operator[](std::size_t index)
      requires(!IsConstAccessor)
    {
      return (*range)[width * index + column];
    }
    [[nodiscard]] const_reference operator[](std::size_t index) const { return (*range)[width * index + column]; }

    [[nodiscard]] std::size_t size() const { return width; }

   private:
    range_storage range;
    std::size_t column;
    std::size_t width;
  };

  [[nodiscard]] RowAccessor<IsConst> operator[](std::size_t index)
    requires(!IsConst)
  {
    return RowAccessor<IsConst>{range, index, width};
  }
  [[nodiscard]] RowAccessor<true> operator[](std::size_t index) const { return RowAccessor<true>{range, index, width}; }

  [[nodiscard]] R &operator*()
    requires(!IsConst)
  {
    return getRange();
  }
  [[nodiscard]] R *operator->()
    requires(!IsConst)
  {
    return range.get();
  }

  [[nodiscard]] const R &operator*() { return getRange(); }

  [[nodiscard]] std::size_t getWidth() const { return width; }
  [[nodiscard]] R &getRange()
    requires(!IsConst)
  {
    return *range;
  }
  [[nodiscard]] const R &getRange() const { return *range; }

  /**
   *
   * @return height of data (row count)
   */
  [[nodiscard]] std::size_t size() const { return std::ceil(std::ranges::size(*range) / static_cast<float>(width)); }

 private:
  range_storage range{};
  std::size_t width{};
};

/**
 * Convenience function for template deduction.
 */
template<bool IsConst = false>
auto makeView2D(std::ranges::range auto &r, std::size_t width) {
  return View2D<std::decay_t<decltype(r)>, IsConst>{r, width};
}

}// namespace pf

#endif//PF_COMMON_VIEWS_VIEW2D_H
