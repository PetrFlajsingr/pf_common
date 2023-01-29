//
// Created by xflajs00 on 29.01.2023.
//

#include <catch2/catch.hpp>

#define PF_UUID_RUNTIME_VALIDITY_CHECK_ENABLED true
#define PF_UUID_RUNTIME_ASSERT(...)                                                                                    \
  throw std::exception {}
#include <pf_common/UUID.h>

TEST_CASE("UUID ctor", "[uuid][ctor]") {
  SECTION("Default ctor returns nil UUID") {
    constexpr auto uuid = pf::UUID{};
    for (const auto b : uuid.getData()) { REQUIRE(b == std::byte{0}); }
  }
  SECTION("Literal ctor returns correct UUID") {
    constexpr auto uuid = pf::UUID{"123e4567-e89b-12d3-a456-426614174000"};
    REQUIRE(uuid.getData()[0] == std::byte{0x12});
    REQUIRE(uuid.getData()[1] == std::byte{0x3e});
    REQUIRE(uuid.getData()[2] == std::byte{0x45});
    REQUIRE(uuid.getData()[3] == std::byte{0x67});
    REQUIRE(uuid.getData()[4] == std::byte{0xe8});
    REQUIRE(uuid.getData()[5] == std::byte{0x9b});
    REQUIRE(uuid.getData()[6] == std::byte{0x12});
    REQUIRE(uuid.getData()[7] == std::byte{0xd3});
    REQUIRE(uuid.getData()[8] == std::byte{0xa4});
    REQUIRE(uuid.getData()[9] == std::byte{0x56});
    REQUIRE(uuid.getData()[10] == std::byte{0x42});
    REQUIRE(uuid.getData()[11] == std::byte{0x66});
    REQUIRE(uuid.getData()[12] == std::byte{0x14});
    REQUIRE(uuid.getData()[13] == std::byte{0x17});
    REQUIRE(uuid.getData()[14] == std::byte{0x40});
    REQUIRE(uuid.getData()[15] == std::byte{0x00});
  }
  SECTION("string_view ctor returns correct UUID") {
    constexpr auto uuid = pf::UUID{std::string_view{"123e4567-e89b-12d3-a456-426614174000"}};
    REQUIRE(uuid.getData()[0] == std::byte{0x12});
    REQUIRE(uuid.getData()[1] == std::byte{0x3e});
    REQUIRE(uuid.getData()[2] == std::byte{0x45});
    REQUIRE(uuid.getData()[3] == std::byte{0x67});
    REQUIRE(uuid.getData()[4] == std::byte{0xe8});
    REQUIRE(uuid.getData()[5] == std::byte{0x9b});
    REQUIRE(uuid.getData()[6] == std::byte{0x12});
    REQUIRE(uuid.getData()[7] == std::byte{0xd3});
    REQUIRE(uuid.getData()[8] == std::byte{0xa4});
    REQUIRE(uuid.getData()[9] == std::byte{0x56});
    REQUIRE(uuid.getData()[10] == std::byte{0x42});
    REQUIRE(uuid.getData()[11] == std::byte{0x66});
    REQUIRE(uuid.getData()[12] == std::byte{0x14});
    REQUIRE(uuid.getData()[13] == std::byte{0x17});
    REQUIRE(uuid.getData()[14] == std::byte{0x40});
    REQUIRE(uuid.getData()[15] == std::byte{0x00});
  }
  SECTION("invalid input fails") {
    REQUIRE_THROWS(pf::UUID{std::string_view{"123e4111567-e89b-12d3-a456-426614174000"}});
    REQUIRE_THROWS(pf::UUID{std::string_view{"123e4567-e89b-12d3-a456-4266141740"}});
    REQUIRE_THROWS(pf::UUID{std::string_view{"X23e4567-e89b-12d3-a456-426614174000"}});
    REQUIRE_THROWS(pf::UUID{std::string_view{"123e4567_e89b-12d3-a456-426614174000"}});
  }
  SECTION("comparison works") {
    constexpr auto uuid1 = pf::UUID{"123e4567-e89b-12d3-a456-426614174000"};
    constexpr auto uuid2 = pf::UUID{"123e4567-e89b-12d3-a456-426614174000"};
    constexpr auto uuid3 = uuid2;
    constexpr auto nilUuid = pf::UUID{};
    REQUIRE(uuid1 == uuid2);
    REQUIRE(uuid1 == uuid3);
    REQUIRE(uuid2 == uuid3);

    REQUIRE(nilUuid != uuid1);
    REQUIRE(nilUuid != uuid2);
    REQUIRE(nilUuid != uuid3);
  }
  SECTION("string conversion") {
    REQUIRE(to_string(pf::UUID{}) == "00000000-0000-0000-0000-000000000000");
    REQUIRE(to_string(pf::UUID{"123e4567-e89b-12d3-a456-426614174000"}) == "123e4567-e89b-12d3-a456-426614174000");
  }
  SECTION("UDL") {
    using namespace pf::uuid_literals;
    [[maybe_unused]] constexpr auto uchar = "00000000-0000-0000-0000-000000000000"_uuid;
    [[maybe_unused]] constexpr auto uwchar = L"00000000-0000-0000-0000-000000000000"_uuid;
    [[maybe_unused]] constexpr auto uchar8 = u8"00000000-0000-0000-0000-000000000000"_uuid;
    [[maybe_unused]] constexpr auto uchar16 = u"00000000-0000-0000-0000-000000000000"_uuid;
    [[maybe_unused]] constexpr auto uchar32 = U"00000000-0000-0000-0000-000000000000"_uuid;
    SUCCEED();
  }
}