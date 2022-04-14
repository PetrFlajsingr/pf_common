//
// Created by petr on 4/17/21.
//

#include <catch2/catch.hpp>
#include <concepts>
#include <pf_common/concepts/StringConvertible.h>
#include <pf_common/enums.h>
using namespace pf::enum_operators;
enum SimpleEnum {};
namespace EnumNs {
enum class ScopedEnum { A = 0x1, B = 0x2, C = 0x3 };
enum class PureScopedEnum { A = 0x1, B = 0x2, C = 0x4 };
}// namespace EnumNs
using namespace EnumNs;
ENABLE_PF_ENUM_OUT_FOR_NAMESPACE(EnumNs)
TEST_CASE("enum flags", "[enums][Flags]") {

  SECTION("supports both simple and scoped enums") {
    REQUIRE(std::same_as<SimpleEnum, pf::Flags<SimpleEnum>::EnumType>);
    REQUIRE(std::same_as<ScopedEnum, pf::Flags<ScopedEnum>::EnumType>);
  }
  SECTION("flags are correctly set after construction") {
    auto flags = pf::Flags<ScopedEnum>{ScopedEnum::A, ScopedEnum::B};
    REQUIRE(flags.is(ScopedEnum::A));
    REQUIRE(flags.is(ScopedEnum::B));
  }

  SECTION("enum operators create correct flags") {
    REQUIRE((ScopedEnum::A | ScopedEnum::B).is(ScopedEnum::A));
    REQUIRE((ScopedEnum::A | ScopedEnum::B).is(ScopedEnum::B));
    REQUIRE((ScopedEnum::A & ScopedEnum::C).is(ScopedEnum::A));
    REQUIRE(!(ScopedEnum::A & ScopedEnum::C).is(ScopedEnum::B));
  }
  SECTION("operators | work for flags and enum value") {
    auto flags = pf::Flags<ScopedEnum>{};
    REQUIRE((flags | ScopedEnum::A).is(ScopedEnum::A));
    REQUIRE((flags | ScopedEnum::B).is(ScopedEnum::B));
    flags |= ScopedEnum::A;
    REQUIRE(flags.is(ScopedEnum::A));
    REQUIRE(!flags.is(ScopedEnum::B));
    flags |= ScopedEnum::B;
    REQUIRE(flags.is(ScopedEnum::B));
  }
  SECTION("operators & work for flags and enum value") {
    auto flags = pf::Flags<ScopedEnum>{ScopedEnum::A};
    REQUIRE((flags & ScopedEnum::A).is(ScopedEnum::A));
    REQUIRE(!(flags & ScopedEnum::B).is(ScopedEnum::B));
    flags = ScopedEnum::A | ScopedEnum::B;
    flags &= ~ScopedEnum::A;
    REQUIRE(!flags.is(ScopedEnum::A));
    REQUIRE(flags.is(ScopedEnum::B));
    REQUIRE(!flags.is(ScopedEnum::C));
  }
  SECTION("operators ^ work for flags and enum value") {
    auto flags = pf::Flags<ScopedEnum>{ScopedEnum::B};
    REQUIRE((flags ^ ScopedEnum::A).is(ScopedEnum::A));
    REQUIRE(!(flags ^ ScopedEnum::B).is(ScopedEnum::B));
    flags ^= ScopedEnum::A;
    REQUIRE(flags.is(ScopedEnum::A));
    REQUIRE(flags.is(ScopedEnum::B));
  }
  SECTION("operators | work for flags and flags") {
    auto flags = pf::Flags<ScopedEnum>{};
    REQUIRE((flags | pf::Flags<ScopedEnum>{ScopedEnum::A}).is(ScopedEnum::A));
    REQUIRE((flags | pf::Flags<ScopedEnum>{ScopedEnum::B}).is(ScopedEnum::B));
    flags |= pf::Flags<ScopedEnum>{ScopedEnum::A};
    flags |= pf::Flags<ScopedEnum>{ScopedEnum::B};
    REQUIRE(flags.is(ScopedEnum::A));
    REQUIRE(flags.is(ScopedEnum::B));
  }
  SECTION("operators & work for flags and flags") {
    auto flags = pf::Flags<ScopedEnum>{ScopedEnum::A};
    REQUIRE((flags & pf::Flags<ScopedEnum>{ScopedEnum::A}).is(ScopedEnum::A));
    REQUIRE(!(flags & pf::Flags<ScopedEnum>{ScopedEnum::B}).is(ScopedEnum::B));
    flags &= ~pf::Flags<ScopedEnum>{ScopedEnum::A};
    REQUIRE(!flags.is(ScopedEnum::A));
    REQUIRE(!flags.is(ScopedEnum::B));
  }
  SECTION("operators ^ work for flags and flags") {
    auto flags = pf::Flags<ScopedEnum>{ScopedEnum::B};
    REQUIRE((flags ^ pf::Flags<ScopedEnum>{ScopedEnum::A}).is(ScopedEnum::A));
    REQUIRE(!(flags ^ pf::Flags<ScopedEnum>{ScopedEnum::B}).is(ScopedEnum::B));
    flags ^= pf::Flags<ScopedEnum>{ScopedEnum::A};
    REQUIRE(flags.is(ScopedEnum::A));
    REQUIRE(flags.is(ScopedEnum::B));
  }
  SECTION("getSetFlags") {
    auto flags = pf::Flags<ScopedEnum>{ScopedEnum::B};
    auto setFlags = flags.getSetFlags();
    for (auto flag : setFlags) { REQUIRE(flag == ScopedEnum::B); }
    flags = ScopedEnum::A | ScopedEnum::B;
    auto setFlags2 = flags.getSetFlags();
    auto iter = setFlags2.begin();
    REQUIRE(iter != setFlags2.end());
    REQUIRE(*iter == ScopedEnum::A);
    REQUIRE(++iter != setFlags2.end());
    REQUIRE(*iter == ScopedEnum::B);
    REQUIRE(++iter != setFlags2.end());
    REQUIRE(*iter == ScopedEnum::C);
  }
  SECTION("pure enum flag detection") {
    REQUIRE(!pf::IsPureFlags<ScopedEnum>());
    REQUIRE(pf::IsPureFlags<PureScopedEnum>());
  }
  SECTION("enum to string") {
    REQUIRE(pf::toString(ScopedEnum::A) == "A");
    REQUIRE(pf::toString(ScopedEnum::B) == "B");
  }
}

TEST_CASE("constexpr enum flags", "[enums][Flags]") {

  SECTION("supports both simple and scoped enums") {
    REQUIRE(std::same_as<SimpleEnum, pf::Flags<SimpleEnum>::EnumType>);
    REQUIRE(std::same_as<ScopedEnum, pf::Flags<ScopedEnum>::EnumType>);
  }
  SECTION("flags are correctly set after construction") {
    constexpr auto flags = pf::Flags<ScopedEnum>{ScopedEnum::A, ScopedEnum::B};
    constexpr auto isA = flags.is(ScopedEnum::A);
    constexpr auto isB = flags.is(ScopedEnum::B);
    static_assert(isA);
    static_assert(isB);
    SUCCEED();
  }

  SECTION("enum operators create correct flags") {
    constexpr bool b1 = (ScopedEnum::A | ScopedEnum::B).is(ScopedEnum::A);
    constexpr bool b2 = (ScopedEnum::A | ScopedEnum::B).is(ScopedEnum::B);
    constexpr bool b3 = (ScopedEnum::A & ScopedEnum::C).is(ScopedEnum::A);
    constexpr bool b4 = !(ScopedEnum::A & ScopedEnum::C).is(ScopedEnum::B);
    static_assert(b1);
    static_assert(b2);
    static_assert(b3);
    static_assert(b4);
    SUCCEED();
  }
}
