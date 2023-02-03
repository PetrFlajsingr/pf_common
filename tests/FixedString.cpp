//
// Created by petr.flajsingr on 2/7/2022.
//
#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <pf_common/containers/FixedString.h>

using namespace pf;
using namespace pf::fixed_string_literal;

TEST_CASE("Fixed string", "[FixedString][char]") {

  SECTION("ctors") {
    constexpr auto a = FixedString{"literal"};
    constexpr auto b = FixedString{""};
    constexpr auto c = FixedString<0>{};
    REQUIRE(std::char_traits<char>::compare(a.data(), "literal", a.size()) == 0);
    REQUIRE(std::char_traits<char>::compare(b.data(), "", b.size()) == 0);
    REQUIRE(std::char_traits<char>::compare(c.data(), "", c.size()) == 0);
  }
  SECTION("UDL") {
    constexpr auto a = "literal"_fstr;
    constexpr auto b = ""_fstr;
    constexpr auto c = FixedString<0>{};
    REQUIRE(std::char_traits<char>::compare(a.data(), "literal", a.size()) == 0);
    REQUIRE(std::char_traits<char>::compare(b.data(), "", b.size()) == 0);
    REQUIRE(std::char_traits<char>::compare(c.data(), "", c.size()) == 0);
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


TEST_CASE("Fixed string char8", "[FixedString][char8_t]") {

  SECTION("ctors") {
    constexpr auto a = FixedU8String{u8"literal"};
    constexpr auto b = FixedU8String{u8""};
    constexpr auto c = FixedU8String<0>{};
    REQUIRE(std::char_traits<char8_t>::compare(a.data(), u8"literal", a.size()) == 0);
    REQUIRE(std::char_traits<char8_t>::compare(b.data(), u8"", b.size()) == 0);
    REQUIRE(std::char_traits<char8_t>::compare(c.data(), u8"", c.size()) == 0);
  }
  SECTION("UDL") {
    constexpr auto a = u8"literal"_fstr;
    constexpr auto b = u8""_fstr;
    constexpr auto c = FixedU8String<0>{};
    REQUIRE(std::char_traits<char8_t>::compare(a.data(), u8"literal", a.size()) == 0);
    REQUIRE(std::char_traits<char8_t>::compare(b.data(), u8"", b.size()) == 0);
    REQUIRE(std::char_traits<char8_t>::compare(c.data(), u8"", c.size()) == 0);
  }
  SECTION("string_view_type") {
    constexpr auto a = FixedU8String{u8"literal"};
    constexpr auto b = FixedU8String{u8""};
    constexpr auto c = FixedU8String<0>{};
    REQUIRE(static_cast<std::u8string_view>(a) == u8"literal");
    REQUIRE(static_cast<std::u8string_view>(b) == u8"");
    REQUIRE(static_cast<std::u8string_view>(c) == u8"");
  }
  SECTION("append") {
    {
      constexpr auto a = FixedU8String{u8"literal"};
      constexpr auto b = FixedU8String{u8""};
      constexpr auto ab = a.append(b);
      REQUIRE(static_cast<std::u8string_view>(ab) == u8"literal");
    }
    {
      constexpr auto a = FixedU8String{u8"literal"};
      constexpr auto b = FixedU8String{u8"literal2"};
      constexpr auto ab = a.append(b);
      REQUIRE(static_cast<std::u8string_view>(ab) == u8"literalliteral2");
    }
    {
      constexpr auto a = FixedU8String{u8"literal"};
      constexpr auto ab = a.append(u8"");
      REQUIRE(static_cast<std::u8string_view>(ab) == u8"literal");
    }
    {
      constexpr auto a = FixedU8String{u8"literal"};
      constexpr auto ab = a.append(u8"literal2");
      REQUIRE(static_cast<std::u8string_view>(ab) == u8"literalliteral2");
    }
  }
  SECTION("insert") {
    {
      constexpr auto a = FixedU8String{u8"literal"};
      constexpr auto b = FixedU8String{u8""};
      constexpr auto ab = a.insert(0, b);
      REQUIRE(static_cast<std::u8string_view>(ab) == u8"literal");
    }
    {
      constexpr auto a = FixedU8String{u8"literal"};
      constexpr auto b = FixedU8String{u8"literal2"};
      constexpr auto ab = a.insert(0, b);
      REQUIRE(static_cast<std::u8string_view>(ab) == u8"literal2literal");
    }
    {
      auto a = FixedU8String{u8"literal"};
      auto ab = a.insert(0, u8"");
      REQUIRE(static_cast<std::u8string_view>(ab) == u8"literal");
    }
    {
      constexpr auto a = FixedU8String{u8"literal"};
      constexpr auto ab = a.insert(0, u8"literal2");
      REQUIRE(static_cast<std::u8string_view>(ab) == u8"literal2literal");
    }
  }
  SECTION("push_back") {
    constexpr auto a = FixedU8String{u8"literal"};
    constexpr auto b = a.push_back(u8'_');
    REQUIRE(static_cast<std::u8string_view>(b) == u8"literal_");
  }
  SECTION("pop_back") {
    constexpr auto a = FixedU8String{u8"literal_"};
    constexpr auto b = a.pop_back();
    REQUIRE(static_cast<std::u8string_view>(b) == u8"literal");
  }
  SECTION("operator+") {
    {
      constexpr auto a = FixedU8String{u8"literal"};
      constexpr auto b = FixedU8String{u8""};
      constexpr auto ab = a + b;
      REQUIRE(static_cast<std::u8string_view>(ab) == u8"literal");
    }
    {
      constexpr auto a = FixedU8String{u8"literal"};
      constexpr auto b = FixedU8String{u8"literal2"};
      constexpr auto ab = a + b;
      REQUIRE(static_cast<std::u8string_view>(ab) == u8"literalliteral2");
    }
    {
      auto a = FixedU8String{u8"literal"};
      auto ab = a + u8"";
      REQUIRE(static_cast<std::u8string_view>(ab) == u8"literal");
    }
    {
      constexpr auto a = FixedU8String{u8"literal"};
      constexpr auto ab = a + u8"literal2";
      REQUIRE(static_cast<std::u8string_view>(ab) == u8"literalliteral2");
    }
  }
}