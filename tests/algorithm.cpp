//
// Created by Petr on 11.11.2020.
//

#include <catch2/catch.hpp>
#include <pf_common/math/Range.h>
#include <pf_common/algorithms.h>
#include <unordered_set>

template<typename T, template<class> typename C>
struct CheckIsInInstantiation {
  static constexpr bool valid = std::same_as<decltype(pf::isIn(std::declval<T>(), std::declval<C<T>>())), bool>;
};

TEST_CASE("is in", "[algorithm][isIn]") {
  const auto vec = std::vector<int>{0, 1, 2, 3, 4, 5, 6};

  SECTION("correctly detects present elements") {
    for (int i = 0; i < static_cast<int>(vec.size()); ++i) { REQUIRE(pf::isIn(i, vec)); }
  }

  SECTION("correctly detects absent elements") {
    for (int i = static_cast<int>(vec.size()); i < 100; ++i) { REQUIRE(!pf::isIn(i, vec)); }
  }

  SECTION("doesn't modify container") {
    auto vecCopy = vec;
    for (int i = 0; i < 100; ++i) { [[maybe_unused]] auto tmp = pf::isIn(i, vecCopy); }
    REQUIRE(vecCopy == vec);
  }

  SECTION("can be instantiated with arbitrary iterable") {
    REQUIRE(CheckIsInInstantiation<int, std::vector>::valid);
    REQUIRE(CheckIsInInstantiation<int, std::unordered_set>::valid);
  }
}

TEST_CASE("find first common", "[algorithm][findFirstCommon]") {
  const auto vec1 = std::vector<int>{0, 1, 2, 3, 4, 5, 6};
  const auto vec2 = std::vector<int>{10, 11, 2, 3, 4, 5, 6};
  const auto vec3 = std::vector<int>{10, 11, 12, 13, 14, 15, 16};

  SECTION("returns nullopt on no common value found") { REQUIRE(!pf::findFirstCommon(vec1, vec3).has_value()); }

  SECTION("returns first common element when one is present") { REQUIRE(*pf::findFirstCommon(vec1, vec2) == 2); }
}
