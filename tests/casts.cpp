//
// Created by xflajs00 on 30.01.2023.
//


#include <catch2/catch_test_macros.hpp>

#define PF_NARROW_FAIL(source, target) throw std::exception()
#define PF_POLYMORPHIC_DOWNCAST_FAIL(msg) throw std::exception()

#include <pf_common/casts.h>

TEST_CASE("narrow", "[narrow]") {
  SECTION("does not fail on correct input") {
    const auto v1 = std::uint8_t {10};
    const auto v2 = pf::narrow<std::uint16_t>(v1);
    REQUIRE(v1 == v2);

    const auto v3 = std::uint16_t {10};
    const auto v4 = pf::narrow<std::uint8_t>(v1);
    REQUIRE(v3 == v4);
  }
  SECTION("fails on incorrect input") {
    REQUIRE_THROWS(pf::narrow<std::uint16_t>(1.2f));
    REQUIRE_THROWS(pf::narrow<std::uint16_t>(std::numeric_limits<std::uint32_t>::max()));
  }
}

TEST_CASE("polymorphic_downcast", "[polymorphic_downcast]") {
  struct Base {
    virtual ~Base() = default;
  };
  struct Derived : public Base {};
  struct Derived2 : public Base {};

  SECTION("does not fail on correct input") {
    const auto ptr = static_cast<Base*>(new Derived{});
    REQUIRE(pf::polymorphic_downcast<Derived*>(ptr) == ptr);
    delete ptr;
  }
  SECTION("fails on incorrect input") {
    const auto ptr = static_cast<Base*>(new Derived{});
    REQUIRE_THROWS(pf::polymorphic_downcast<Derived2*>(ptr) == ptr);
    delete ptr;
  }
}