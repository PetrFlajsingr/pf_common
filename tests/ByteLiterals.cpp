//
// Created by Petr on 11.11.2020.
//

#include <catch2/catch_test_macros.hpp>
#include <pf_common/ByteLiterals.h>

TEST_CASE("byte literals", "[byte_literals]") {
  using namespace pf::byte_literals;
  REQUIRE(1_KB == 1000_B);
  REQUIRE(1_MB == 1000_KB);
  REQUIRE(1_GB == 1000_MB);
  REQUIRE(1_TB == 1000_GB);
  REQUIRE(1_PB == 1000_TB);

  REQUIRE(1_KiB == 1024_B);
  REQUIRE(1_MiB == 1024_KiB);
  REQUIRE(1_GiB == 1024_MiB);
  REQUIRE(1_TiB == 1024_GiB);
  REQUIRE(1_PiB == 1024_TiB);
}
