//
// Created by Petr on 11.11.2020.
//

#include <catch2/catch.hpp>
#include <pf_common/RAII.h>

TEST_CASE("RAII", "[RAII]") {
  auto testBool = true;
  {
    const auto raii = pf::RAII([&] { testBool = false; });
    REQUIRE(testBool);
  }
  REQUIRE(!testBool);
}

TEST_CASE("RAII exception", "[RAII]") {
  auto testBool = true;
  try {
    const auto raii = pf::RAII([&] { testBool = false; });
    REQUIRE(testBool);
    throw 0;
  } catch (...) {}
  REQUIRE(!testBool);
}
