//
// Created by petr on 4/22/21.
//

#include <catch2/catch.hpp>
#include <pf_common/array.h>

TEST_CASE("array", "[make_array]") {
  using namespace std::string_literals;
  auto array = pf::make_array<int>(1, 2, 3, 4, 5);
  REQUIRE(array.size() == 5);
  REQUIRE(std::same_as<decltype(array)::value_type, int>);
  auto strArray = pf::make_array<std::string>("1", "2", "3", "4", "5");
  REQUIRE(strArray.size() == 5);
  REQUIRE(std::same_as<decltype(strArray)::value_type, std::string>);
}
