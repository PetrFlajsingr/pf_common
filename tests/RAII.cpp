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
