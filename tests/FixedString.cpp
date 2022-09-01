//
// Created by petr.flajsingr on 2/7/2022.
//
#include <catch2/catch.hpp>
#include <pf_common/FixedString.h>
/* TODO: fix this
TEST_CASE("Fixed string", "[FixedString]") {

  SECTION("Works in constexpr context") {
    using namespace pf::FixedStringLiteral;
    constexpr auto a = "literal"_fixed_str;
    constexpr auto equals = (a == "literal");
    constexpr auto notEquals = (a != "a");
    REQUIRE(equals);
    REQUIRE(notEquals);

    constexpr auto b = "2"_fixed_str;
    constexpr auto ab = a + b;
    constexpr auto concatWorks = (ab == "literal2");
    REQUIRE(concatWorks);
  }
  SECTION("Works in runtime context") {
    using namespace pf::FixedStringLiteral;
    const auto a = "literal"_fixed_str;
    const auto equals = (a == "literal");
    const auto notEquals = (a != "a");
    REQUIRE(equals);
    REQUIRE(notEquals);

    const auto b = "2"_fixed_str;
    const auto ab = a + b;
    const auto concatWorks = (ab == "literal2");
    REQUIRE(concatWorks);

  }

}*/