//
// Created by Petr on 11.11.2020.
//

#include <catch2/catch.hpp>
#include <pf_common/ObjectPool.h>

TEST_CASE("ObjectPool", "[ObjectPool]") {

  SECTION("can lend up to PoolSize objects") {
    constexpr auto poolSize = 100;
    auto pool = pf::ObjectPool<int, poolSize, pf::PoolAllocStrategy::Preallocate>();
    auto borrowed = std::vector<std::experimental::observer_ptr<int>>();

    for (std::size_t i = 0; i < poolSize; ++i) {
      REQUIRE_NOTHROW(borrowed.emplace_back(pool.lease()));
    }
    REQUIRE_THROWS(pool.lease());
  }

  SECTION("recycles returned objects") {
    constexpr auto poolSize = 100;
    auto pool = pf::ObjectPool<int, poolSize, pf::PoolAllocStrategy::Preallocate>();
    auto borrowed = std::vector<std::experimental::observer_ptr<int>>();

    for (std::size_t i = 0; i < poolSize; ++i) {
      REQUIRE_NOTHROW(borrowed.emplace_back(pool.lease()));
    }
    REQUIRE(pool.available() == 0);
    REQUIRE(pool.used() == poolSize);
    for (std::size_t i = 0; i < poolSize; ++i) { REQUIRE_NOTHROW(pool.release(borrowed[i])); }
    borrowed.clear();
    REQUIRE(pool.used() == 0);
    REQUIRE(pool.available() == poolSize);
  }
}