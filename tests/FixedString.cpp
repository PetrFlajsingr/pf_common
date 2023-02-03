//
// Created by petr.flajsingr on 2/7/2022.
//
#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <pf_common/containers/FixedString.h>

using namespace pf;

TEST_CASE("Fixed string", "[FixedString]") {

  SECTION("ctors") {
    constexpr auto a = FixedString{"literal"};
    constexpr auto b = FixedString{""};
    constexpr auto c = FixedString<0>{};
    REQUIRE(std::strcmp(a.data(), "literal") == 0);
    REQUIRE(std::strcmp(b.data(), "") == 0);
    REQUIRE(std::strcmp(c.data(), "") == 0);
  }
  SECTION("string_view_type") {
    constexpr auto a = FixedString{"literal"};
    constexpr auto b = FixedString{""};
    constexpr auto c = FixedString<0>{};
    REQUIRE(static_cast<std::string_view>(a) == "literal");
    REQUIRE(static_cast<std::string_view>(b) == "");
    REQUIRE(static_cast<std::string_view>(c) == "");
  }
  SECTION("append") {
    {
      constexpr auto a = FixedString{"literal"};
      constexpr auto b = FixedString{""};
      constexpr auto ab = a.append(b);
      REQUIRE(static_cast<std::string_view>(ab) == "literal");
    }
    {
      constexpr auto a = FixedString{"literal"};
      constexpr auto b = FixedString{"literal2"};
      constexpr auto ab = a.append(b);
      REQUIRE(static_cast<std::string_view>(ab) == "literalliteral2");
    }
    {
      constexpr auto a = FixedString{"literal"};
      constexpr auto ab = a.append("");
      REQUIRE(static_cast<std::string_view>(ab) == "literal");
    }
    {
      constexpr auto a = FixedString{"literal"};
      constexpr auto ab = a.append("literal2");
      REQUIRE(static_cast<std::string_view>(ab) == "literalliteral2");
    }
  }
  SECTION("insert") {
    {
      constexpr auto a = FixedString{"literal"};
      constexpr auto b = FixedString{""};
      constexpr auto ab = a.insert(0, b);
      REQUIRE(static_cast<std::string_view>(ab) == "literal");
    }
    {
      constexpr auto a = FixedString{"literal"};
      constexpr auto b = FixedString{"literal2"};
      constexpr auto ab = a.insert(0, b);
      REQUIRE(static_cast<std::string_view>(ab) == "literal2literal");
    }
    {
      auto a = FixedString{"literal"};
      auto ab = a.insert(0, "");
      REQUIRE(static_cast<std::string_view>(ab) == "literal");
    }
    {
      constexpr auto a = FixedString{"literal"};
      constexpr auto ab = a.insert(0, "literal2");
      REQUIRE(static_cast<std::string_view>(ab) == "literal2literal");
    }
  }
  SECTION("push_back") {
    constexpr auto a = FixedString{"literal"};
    constexpr auto b = a.push_back('_');
    REQUIRE(static_cast<std::string_view>(b) == "literal_");
  }
  SECTION("pop_back") {
    constexpr auto a = FixedString{"literal_"};
    constexpr auto b = a.pop_back();
    REQUIRE(static_cast<std::string_view>(b) == "literal");
  }
  SECTION("operator+") {
    {
      constexpr auto a = FixedString{"literal"};
      constexpr auto b = FixedString{""};
      constexpr auto ab = a + b;
      REQUIRE(static_cast<std::string_view>(ab) == "literal");
    }
    {
      constexpr auto a = FixedString{"literal"};
      constexpr auto b = FixedString{"literal2"};
      constexpr auto ab = a + b;
      REQUIRE(static_cast<std::string_view>(ab) == "literalliteral2");
    }
    {
      auto a = FixedString{"literal"};
      auto ab = a + "";
      REQUIRE(static_cast<std::string_view>(ab) == "literal");
    }
    {
      constexpr auto a = FixedString{"literal"};
      constexpr auto ab = a + "literal2";
      REQUIRE(static_cast<std::string_view>(ab) == "literalliteral2");
    }
  }
}