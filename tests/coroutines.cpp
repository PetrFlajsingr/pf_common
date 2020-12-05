//
// Created by Petr on 11.11.2020.
//

#include <catch2/catch.hpp>
#include <pf_common/coroutines/Sequence.h>

TEST_CASE("iota", "[iota]") {
  SECTION("default argument") {
    auto iota = pf::iota<int>();
    for (int i = 0; i < 100; ++i) { REQUIRE(i == pf::getNext(iota)); }
  }
  SECTION("explicit argument") {
    auto iota = pf::iota<int>(10);
    for (int i = 10; i < 100; ++i) { REQUIRE(i == pf::getNext(iota)); }
  }
}

TEST_CASE("mod repeat", "[mod_repeat]") {
  SECTION("default argument") {
    auto iota = pf::modRepeat(10);
    for (int i = 0; i < 100; ++i) { REQUIRE(i%10 == pf::getNext(iota)); }
  }
  SECTION("explicit argument") {
    auto iota = pf::modRepeat<int>(10, 10);
    for (int i = 10; i < 100; ++i) { REQUIRE(i%10 == pf::getNext(iota)); }
  }
}
