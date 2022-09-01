//
// Created by Petr on 11.11.2020.
//

#include <catch2/catch.hpp>
#include <pf_common/LazyInit.h>

TEST_CASE("LazyInit", "[LazyInit]") {
  SECTION("returns correct calculated value") {
    constexpr int result = 10;
    auto init = pf::LazyInit<int>([&] { return result; });
    REQUIRE(result == *init);
  }
  SECTION("evaluation is done only once") {
    auto cnt = 0;
    constexpr int result = 10;
    auto init = pf::LazyInit<int>([&] {
      ++cnt;
      return result;
    });
    REQUIRE(result == *init);
    REQUIRE(result == *init);
    REQUIRE(result == *init);
    REQUIRE(cnt == 1);
  }
}
