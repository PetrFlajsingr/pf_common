//
// Created by petr on 4/17/21.
//

#include <catch2/catch.hpp>
#include <concepts>
#include <pf_common/enums.h>

TEST_CASE("enum flags", "[enums][Flags]") {
  using namespace pf::enum_flag_operators;
  enum SimpleEnum {};
  enum class ScopedEnum { A = 0x1, B = 0x2, C = 0x3 };
  SECTION("supports both simple and scoped enums") {
    REQUIRE(std::same_as<SimpleEnum, pf::Flags<SimpleEnum>::EnumType>);
    REQUIRE(std::same_as<ScopedEnum, pf::Flags<ScopedEnum>::EnumType>);
  }
  SECTION("flags are correctly set after construction") {
    auto flags = pf::Flags<ScopedEnum> { ScopedEnum::A, ScopedEnum::B };
    REQUIRE(flags.is(ScopedEnum::A));
    REQUIRE(flags.is(ScopedEnum::B));
    REQUIRE(false);
  }

  SECTION("enum operators create correct flags") {
      REQUIRE((ScopedEnum::A | ScopedEnum::B).is(ScopedEnum::A));
      REQUIRE((ScopedEnum::A | ScopedEnum::B).is(ScopedEnum::B));
      REQUIRE((ScopedEnum::A & ScopedEnum::C).is(ScopedEnum::A));
      REQUIRE(!(ScopedEnum::A & ScopedEnum::C).is(ScopedEnum::B));
  }
  SECTION("operators | work for flags and enum value") {
    auto flags = pf::Flags<ScopedEnum> {};
    REQUIRE((flags | ScopedEnum::A).is(ScopedEnum::A));
    REQUIRE((flags | ScopedEnum::B).is(ScopedEnum::B));
    flags |= ScopedEnum::A;
    REQUIRE(flags.is(ScopedEnum::A));
    REQUIRE(!flags.is(ScopedEnum::B));
    flags |= ScopedEnum::B;
    REQUIRE(flags.is(ScopedEnum::B));
  }
  SECTION("operators & work for flags and enum value") {
    auto flags = pf::Flags<ScopedEnum> {ScopedEnum::A};
    REQUIRE((flags & ScopedEnum::A).is(ScopedEnum::A));
    REQUIRE(!(flags & ScopedEnum::B).is(ScopedEnum::B));
    flags = ScopedEnum::A | ScopedEnum::B;
    flags &= ~ScopedEnum::A;
    REQUIRE(!flags.is(ScopedEnum::A));
    REQUIRE(flags.is(ScopedEnum::B));
    REQUIRE(!flags.is(ScopedEnum::C));
  }
  SECTION("operators ^ work for flags and enum value") {
    auto flags = pf::Flags<ScopedEnum> {ScopedEnum::B};
    REQUIRE((flags ^ ScopedEnum::A).is(ScopedEnum::A));
    REQUIRE(!(flags ^ ScopedEnum::B).is(ScopedEnum::B));
    flags ^= ScopedEnum::A;
    REQUIRE(flags.is(ScopedEnum::A));
    REQUIRE(flags.is(ScopedEnum::B));
  }
  SECTION("operators | work for flags and flags") {
    auto flags = pf::Flags<ScopedEnum> {};
    REQUIRE((flags | pf::Flags<ScopedEnum>{ScopedEnum::A}).is(ScopedEnum::A));
    REQUIRE((flags | pf::Flags<ScopedEnum>{ScopedEnum::B}).is(ScopedEnum::B));
    flags |= pf::Flags<ScopedEnum>{ScopedEnum::A};
    flags |= pf::Flags<ScopedEnum>{ScopedEnum::B};
    REQUIRE(flags.is(ScopedEnum::A));
    REQUIRE(flags.is(ScopedEnum::B));
  }
  SECTION("operators & work for flags and flags") {
    auto flags = pf::Flags<ScopedEnum> {ScopedEnum::A};
    REQUIRE((flags & pf::Flags<ScopedEnum>{ScopedEnum::A}).is(ScopedEnum::A));
    REQUIRE(!(flags & pf::Flags<ScopedEnum>{ScopedEnum::B}).is(ScopedEnum::B));
    flags &= ~pf::Flags<ScopedEnum>{ScopedEnum::A};
    REQUIRE(!flags.is(ScopedEnum::A));
    REQUIRE(!flags.is(ScopedEnum::B));
  }
  SECTION("operators ^ work for flags and flags") {
    auto flags = pf::Flags<ScopedEnum> {ScopedEnum::B};
    REQUIRE((flags ^ pf::Flags<ScopedEnum>{ScopedEnum::A}).is(ScopedEnum::A));
    REQUIRE(!(flags ^ pf::Flags<ScopedEnum>{ScopedEnum::B}).is(ScopedEnum::B));
    flags ^= pf::Flags<ScopedEnum>{ScopedEnum::A};
    REQUIRE(flags.is(ScopedEnum::A));
    REQUIRE(flags.is(ScopedEnum::B));
  }
}
