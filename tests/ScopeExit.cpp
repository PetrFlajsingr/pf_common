//
// Created by Petr on 11.11.2020.
//

#include <catch2/catch_test_macros.hpp>
#include <pf_common/ScopeExit.h>
#include <unordered_map>


TEST_CASE("ScopeExit", "[ScopeExit]") {
  auto testBool = true;
  {
    const auto raii = pf::ScopeExit([&] { testBool = false; });
    REQUIRE(testBool);
  }
  REQUIRE(!testBool);
}

TEST_CASE("ScopeExit exception", "[ScopeExit]") {
  auto testBool = true;
  try {
    const auto raii = pf::ScopeExit([&] { testBool = false; });
    REQUIRE(testBool);
    throw 0;
  } catch (...) {}
  REQUIRE(!testBool);
}
