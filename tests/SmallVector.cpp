//
// Created by Petr.Flajsingr on 2023-02-03.
//

#include <catch2/catch_test_macros.hpp>
#include <pf_common/containers/SmallVector.h>

struct ThreeWayComparable {
  int a;
  auto operator<=>(const ThreeWayComparable &) const = default;
};
struct OldSchoolComparable {
  int a;
  bool operator==(const OldSchoolComparable &rhs) const { return a == rhs.a; }
  bool operator!=(const OldSchoolComparable &rhs) const { return !(rhs == *this); }
  bool operator<(const OldSchoolComparable &rhs) const { return a < rhs.a; }
  bool operator>(const OldSchoolComparable &rhs) const { return rhs < *this; }
  bool operator<=(const OldSchoolComparable &rhs) const { return !(rhs < *this); }
  bool operator>=(const OldSchoolComparable &rhs) const { return !(*this < rhs); }
};

using namespace pf;

TEST_CASE("SmallVector constructors", "[SmallVector][ctors]") {
  constexpr auto CAPACITY = 10ull;
  constexpr static int INIT_VALUE = 1;
  const std::initializer_list<int> initList{1, 2, 3, 4, 5};

  SECTION("default") {
    const SmallVector<int, CAPACITY> vector{};

    REQUIRE(vector.size() == 0ull);
    REQUIRE(vector.empty());
  }
  SECTION("sized") {
    constexpr static std::size_t SIZE = 5;
    const SmallVector<int, CAPACITY> vector(SIZE);

    REQUIRE(vector.size() == SIZE);
    REQUIRE(!vector.empty());
  }
  SECTION("sized init value") {
    constexpr static std::size_t SIZE = 5;
    const SmallVector<int, CAPACITY> vector(SIZE, INIT_VALUE);

    REQUIRE(vector.size() == SIZE);
    REQUIRE(!vector.empty());
    for (std::size_t i = 0; i < vector.size(); ++i) { REQUIRE(vector[i] == INIT_VALUE); }
  }
  SECTION("iterator") {
    const SmallVector<int, CAPACITY> vector(initList.begin(), initList.end());

    REQUIRE(vector.size() == initList.size());
    REQUIRE(!vector.empty());

    const auto initListIter = initList.begin();
    for (std::size_t i = 0; i < vector.size(); ++i) { REQUIRE(vector[i] == *std::next(initListIter, i)); }
  }
  SECTION("initializer_list") {
    const SmallVector<int, CAPACITY> vector{initList};

    REQUIRE(vector.size() == initList.size());
    REQUIRE(!vector.empty());

    const auto initListIter = initList.begin();
    for (std::size_t i = 0; i < vector.size(); ++i) { REQUIRE(vector[i] == *std::next(initListIter, i)); }
  }
  SECTION("copy") {
    const SmallVector<int, CAPACITY> src{initList};
    const SmallVector<int, CAPACITY> vector{src};

    REQUIRE(vector.size() == src.size());
    REQUIRE(!vector.empty());

    for (std::size_t i = 0; i < vector.size(); ++i) { REQUIRE(vector[i] == src[i]); }
  }
  SECTION("move") {
    SmallVector<int, CAPACITY> src{initList};
    const SmallVector<int, CAPACITY> vector{std::move(src)};

    REQUIRE(vector.size() == initList.size());
    REQUIRE(!vector.empty());

    const auto initListIter = initList.begin();
    for (std::size_t i = 0; i < vector.size(); ++i) { REQUIRE(vector[i] == *std::next(initListIter, i)); }
  }
}


TEST_CASE("SmallVector operators", "[SmallVector]") {
  constexpr static std::size_t CAPACITY = 10;

  SECTION("equality is detected correctly <=>") {

    const SmallVector<ThreeWayComparable, CAPACITY> vec1{ThreeWayComparable{1}, ThreeWayComparable{2},
                                                          ThreeWayComparable{3}, ThreeWayComparable{4},
                                                          ThreeWayComparable{5}};
    const SmallVector<ThreeWayComparable, CAPACITY> vec2 = vec1;

    REQUIRE(vec1 == vec2);
    REQUIRE(!(vec1 != vec2));
  }

  SECTION("equality is detected correctly == !=") {

    const SmallVector<OldSchoolComparable, CAPACITY> vec1{OldSchoolComparable{1}, OldSchoolComparable{2},
                                                           OldSchoolComparable{3}, OldSchoolComparable{4},
                                                           OldSchoolComparable{5}};
    const SmallVector<OldSchoolComparable, CAPACITY> vec2 = vec1;

    REQUIRE(vec1 == vec2);
    REQUIRE(!(vec1 != vec2));
  }

  SECTION("inequality is detected correctly <=>") {

    const SmallVector<ThreeWayComparable, CAPACITY> vec1{ThreeWayComparable{1}, ThreeWayComparable{2},
                                                          ThreeWayComparable{3}, ThreeWayComparable{4},
                                                          ThreeWayComparable{5}};
    const SmallVector<ThreeWayComparable, CAPACITY> vec2{ThreeWayComparable{1}, ThreeWayComparable{2},
                                                          ThreeWayComparable{3}, ThreeWayComparable{4}};

    REQUIRE(vec1 != vec2);
    REQUIRE(!(vec1 == vec2));
  }

  SECTION("inequality is detected correctly == !=") {

    const SmallVector<OldSchoolComparable, CAPACITY> vec1{OldSchoolComparable{1}, OldSchoolComparable{2},
                                                           OldSchoolComparable{3}, OldSchoolComparable{4},
                                                           OldSchoolComparable{5}};
    const SmallVector<OldSchoolComparable, CAPACITY> vec2{OldSchoolComparable{1}, OldSchoolComparable{2},
                                                           OldSchoolComparable{3}, OldSchoolComparable{4}};

    REQUIRE(vec1 != vec2);
    REQUIRE(!(vec1 == vec2));
  }

  SECTION("greater is detected correctly <=>") {

    const SmallVector<ThreeWayComparable, CAPACITY> vec1{ThreeWayComparable{5}, ThreeWayComparable{2},
                                                          ThreeWayComparable{3}, ThreeWayComparable{4},
                                                          ThreeWayComparable{5}};
    const SmallVector<ThreeWayComparable, CAPACITY> vec2{ThreeWayComparable{1}, ThreeWayComparable{2},
                                                          ThreeWayComparable{3}, ThreeWayComparable{4}};

    REQUIRE(vec1 > vec2);
    REQUIRE(!(vec1 < vec2));
  }

  SECTION("greater is detected correctly >") {

    const SmallVector<OldSchoolComparable, CAPACITY> vec1{OldSchoolComparable{5}, OldSchoolComparable{2},
                                                           OldSchoolComparable{3}, OldSchoolComparable{4},
                                                           OldSchoolComparable{5}};
    const SmallVector<OldSchoolComparable, CAPACITY> vec2{OldSchoolComparable{1}, OldSchoolComparable{2},
                                                           OldSchoolComparable{3}, OldSchoolComparable{4}};

    REQUIRE(vec1 > vec2);
    REQUIRE(!(vec1 < vec2));
  }

  SECTION("greater or equal is detected correctly <=>") {

    const SmallVector<ThreeWayComparable, CAPACITY> vec1{ThreeWayComparable{5}, ThreeWayComparable{2},
                                                          ThreeWayComparable{3}, ThreeWayComparable{4},
                                                          ThreeWayComparable{5}};
    const SmallVector<ThreeWayComparable, CAPACITY> vec2{ThreeWayComparable{1}, ThreeWayComparable{2},
                                                          ThreeWayComparable{3}, ThreeWayComparable{4}};
    const auto vec3 = vec1;

    REQUIRE(vec1 >= vec2);
    REQUIRE(!(vec1 < vec2));

    REQUIRE(vec1 >= vec3);
  }

  SECTION("greater or equal is detected correctly >=") {

    const SmallVector<OldSchoolComparable, CAPACITY> vec1{OldSchoolComparable{5}, OldSchoolComparable{2},
                                                           OldSchoolComparable{3}, OldSchoolComparable{4},
                                                           OldSchoolComparable{5}};
    const SmallVector<OldSchoolComparable, CAPACITY> vec2{OldSchoolComparable{1}, OldSchoolComparable{2},
                                                           OldSchoolComparable{3}, OldSchoolComparable{4}};
    const auto vec3 = vec1;

    REQUIRE(vec1 > vec2);
    REQUIRE(!(vec1 < vec2));
    REQUIRE(vec1 >= vec3);
  }

  SECTION("less is detected correctly <=>") {

    const SmallVector<ThreeWayComparable, CAPACITY> vec1{ThreeWayComparable{1}, ThreeWayComparable{2},
                                                          ThreeWayComparable{3}, ThreeWayComparable{4},
                                                          ThreeWayComparable{5}};
    const SmallVector<ThreeWayComparable, CAPACITY> vec2{ThreeWayComparable{5}, ThreeWayComparable{2},
                                                          ThreeWayComparable{3}, ThreeWayComparable{4}};

    REQUIRE(vec1 < vec2);
    REQUIRE(!(vec1 > vec2));
  }

  SECTION("less is detected correctly >") {

    const SmallVector<OldSchoolComparable, CAPACITY> vec1{OldSchoolComparable{1}, OldSchoolComparable{2},
                                                           OldSchoolComparable{3}, OldSchoolComparable{4},
                                                           OldSchoolComparable{5}};
    const SmallVector<OldSchoolComparable, CAPACITY> vec2{OldSchoolComparable{5}, OldSchoolComparable{2},
                                                           OldSchoolComparable{3}, OldSchoolComparable{4}};

    REQUIRE(vec1 < vec2);
    REQUIRE(!(vec1 > vec2));
  }

  SECTION("less or equal is detected correctly <=>") {

    const SmallVector<ThreeWayComparable, CAPACITY> vec1{ThreeWayComparable{1}, ThreeWayComparable{2},
                                                          ThreeWayComparable{3}, ThreeWayComparable{4},
                                                          ThreeWayComparable{5}};
    const SmallVector<ThreeWayComparable, CAPACITY> vec2{ThreeWayComparable{5}, ThreeWayComparable{2},
                                                          ThreeWayComparable{3}, ThreeWayComparable{4}};
    const auto vec3 = vec1;

    REQUIRE(vec1 <= vec2);
    REQUIRE(!(vec1 > vec2));

    REQUIRE(vec1 <= vec3);
  }

  SECTION("less or equal is detected correctly >=") {

    const SmallVector<OldSchoolComparable, CAPACITY> vec1{OldSchoolComparable{1}, OldSchoolComparable{2},
                                                           OldSchoolComparable{3}, OldSchoolComparable{4},
                                                           OldSchoolComparable{5}};
    const SmallVector<OldSchoolComparable, CAPACITY> vec2{OldSchoolComparable{5}, OldSchoolComparable{2},
                                                           OldSchoolComparable{3}, OldSchoolComparable{4}};
    const auto vec3 = vec1;

    REQUIRE(vec1 < vec2);
    REQUIRE(!(vec1 > vec2));
    REQUIRE(vec1 <= vec3);
  }

  SECTION("copy assignment") {
    const SmallVector<int, CAPACITY> vec1{1, 2, 3, 4, 5};
    const SmallVector<int, CAPACITY> vec2{5, 2, 3, 4};

    SmallVector<int, CAPACITY> vec3{};
    vec3 = vec1;
    REQUIRE(vec3.size() == vec1.size());
    for (std::size_t i = 0; i < vec1.size(); ++i) { REQUIRE(vec1[i] == vec3[i]); }

    vec3 = vec2;
    REQUIRE(vec3.size() == vec2.size());
    for (std::size_t i = 0; i < vec2.size(); ++i) { REQUIRE(vec2[i] == vec3[i]); }
  }
}