//
// Created by Petr.Flajsingr on 2023-02-03.
//

#include <catch2/catch_test_macros.hpp>
#include <pf_common/containers/SmallVector.h>

using namespace pf;

TEST_CASE("SmallVector constructors", "[SmallVector][ctors]") {
  constexpr auto CAPACITY = 10ull;

  SECTION("default") {
    const SmallVector<int, CAPACITY> vector{};

    REQUIRE(vector.size() == 0ull);
    REQUIRE(vector.empty());
    REQUIRE(vector.max_size() == CAPACITY);
    REQUIRE(vector.capacity() == CAPACITY);
  }
}