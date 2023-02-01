//
// Created by petr.flajsingr on 2/7/2022.
//
#include <catch2/catch_test_macros.hpp>
#include <pf_common/FixedString.h>


TEST_CASE("Fixed string", "[FixedString]") {

  SECTION("Works in constexpr context") {
    constexpr auto a = pf::FixedString{"literal"};
    constexpr auto equals = (a == "literal");
    constexpr auto notEquals = (a != "a");
    REQUIRE(equals);
    REQUIRE(notEquals);

    constexpr auto b = pf::FixedString{"2"};
    constexpr auto ab = a + b;
    constexpr auto concatWorks = (ab == "literal2");
    REQUIRE(concatWorks);
  }
  SECTION("Works in runtime context") {
    const auto a = pf::FixedString{"literal"};
    const auto equals = (a == "literal");
    const auto notEquals = (a != "a");
    REQUIRE(equals);
    REQUIRE(notEquals);

    const auto b = pf::FixedString{"2"};
    const auto ab = a + b;
    const auto concatWorks = (ab == "literal2");
    REQUIRE(concatWorks);

  }

}