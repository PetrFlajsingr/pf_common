//
// Created by petr on 4/17/21.
//

#include <catch2/catch_test_macros.hpp>
#include <ostream>
#include <pf_common/concepts/Incrementable.h>
#include <pf_common/concepts/OneOf.h>
#include <pf_common/concepts/PtrConstructible.h>
#include <pf_common/concepts/Serializable.h>
#include <pf_common/concepts/StreamConcepts.h>
#include <pf_common/concepts/StringConvertible.h>
#define PF_ENUM_OUT
#include <pf_common/enums.h>

TEST_CASE("One of", "[concepts][OneOf]") {
  SECTION("type is correctly detected in type list") {
    REQUIRE(pf::OneOf<int, int, bool, double>);
    REQUIRE(pf::OneOf<bool, int, bool, double>);
    REQUIRE(pf::OneOf<double, int, bool, double>);
  }
  SECTION("type is correctly not detected in type list") {
    REQUIRE(!pf::OneOf<char, int, bool, double>);
    REQUIRE(!pf::OneOf<unsigned char, int, bool, double>);
    REQUIRE(!pf::OneOf<std::byte, int, bool, double>);
  }

  SECTION("value is correctly detected in value list") {
    REQUIRE(pf::OneOfValues_v<1, 1, 2, 3>);
    REQUIRE(pf::OneOfValues_v<2, 1, 2, 3>);
    REQUIRE(pf::OneOfValues_v<3, 1, 2, 3>);
  }
  SECTION("value is correctly not detected in value list") {
    REQUIRE(!pf::OneOfValues_v<10, 1, 2, 3>);
    REQUIRE(!pf::OneOfValues_v<11, 1, 2, 3>);
    REQUIRE(!pf::OneOfValues_v<12, 1, 2, 3>);
  }
}

namespace test_details {
struct WithToString {
  std::string toString() const { return "WithToString"; }
};
struct WithStreamOps {
  friend std::ostream &operator<<(std::ostream &os, const WithStreamOps &) {
    os << "WithStreamOps";
    return os;
  }
};
}// namespace test_details

TEST_CASE("String convertible", "[concepts][ToStringConvertible]") {

  SECTION("string convertibles are correctly detected") {
    REQUIRE(pf::ToStringConvertible<std::string>);
    REQUIRE(pf::ToStringConvertible<int>);
    REQUIRE(pf::ToStringConvertible<test_details::WithToString>);
    REQUIRE(pf::ToStringConvertible<test_details::WithStreamOps>);
  }

  SECTION("string convertibles are correctly converted") {
    REQUIRE(pf::toString(std::string("a")) == std::string("a"));
    REQUIRE(pf::toString(10) == std::string("10"));
    REQUIRE(pf::toString(test_details::WithToString{}) == std::string("WithToString"));
    REQUIRE(pf::toString(test_details::WithStreamOps{}) == std::string("WithStreamOps"));
  }

  SECTION("enums can be converted to string") {
    enum class E1 {
      A, B, C
    };
    enum E2 {
      A, B, C
    };
    REQUIRE(pf::toString(E1::A) == std::string("A"));
    REQUIRE(pf::toString(E1::B) == std::string("B"));
    REQUIRE(pf::toString(E1::C) == std::string("C"));
    REQUIRE(pf::toString(E2::A) == std::string("A"));
    REQUIRE(pf::toString(E2::B) == std::string("B"));
    REQUIRE(pf::toString(E2::C) == std::string("C"));
  }
}

struct PtrTest : public pf::PtrConstructible<PtrTest> {
  PtrTest(int value) : value(value) {}
  int value;
};
TEST_CASE("Pointer constructible", "[concepts][PtrConstructible]") {
  SECTION("objects are constructed correctly") {
    REQUIRE(PtrTest::CreateShared(10)->value == 10);
    REQUIRE(PtrTest::CreateUnique(10)->value == 10);
  }
}

struct SerStruct {
  SerStruct(int value) : value(value) {}
  int value;
  std::vector<std::byte> serialize() const {
    auto result = std::vector<std::byte>{};
    std::ranges::copy(reinterpret_cast<const std::byte*>(&value), reinterpret_cast<const std::byte*>(&value)+ 4, std::back_inserter(result));
    return result;
  }
  static SerStruct Deserialize(std::span<const std::byte> vals)  {
    auto result = SerStruct{0};
    std::ranges::copy(vals, reinterpret_cast<std::byte*>(&result.value));
    return result;
  }
};
TEST_CASE("Serialization", "[concepts][Serializable]") {
  SECTION("serializable objects are detected correctly") {
    REQUIRE(pf::Serializable<SerStruct>);
  }
  SECTION("non-serializable objects are detected correctly") {
    REQUIRE(!pf::Serializable<int>);
  }
  SECTION("deserializable objects are detected correctly") {
    REQUIRE(pf::Serializable<SerStruct>);
  }
  SECTION("non-deserializable objects are detected correctly") {
    REQUIRE(!pf::Serializable<int>);
  }

  SECTION("objects can be saved to file and loaded back up") {
    pf::saveToFile("./test.ser", SerStruct{10});
    auto deserialized = pf::loadFromFile<SerStruct>("./test.ser");
    REQUIRE(deserialized.value == 10);
  }
}