//
// Created by xflajs00 on 13.11.2022.
//
#include <array>
#define PF_STATIC_VECTOR_ENABLE_BOUND_CHECKS true
#define PF_STATIC_VECTOR_DEBUG_T_PTR_MEMBER_ENABLE false
#include <catch2/catch_test_macros.hpp>
#include <pf_common/StaticVector.h>

using namespace pf;

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
template<std::size_t I>
struct CtorDtorCounter {
  template<typename... Args>
  explicit(false) CtorDtorCounter(Args...) {
    ++ctorCnt;
    if constexpr (sizeof...(Args) == 0) { ++defaultCtorCnt; }
  }
  CtorDtorCounter(const CtorDtorCounter &) {
    ++copyCtorCnt;
    ++ctorCnt;
  }
  CtorDtorCounter(CtorDtorCounter &&) {
    ++moveCtorCnt;
    ++ctorCnt;
  }
  CtorDtorCounter &operator=(const CtorDtorCounter &) {
    ++copyAssignCnt;
    return *this;
  }
  CtorDtorCounter &operator=(CtorDtorCounter &&) {
    ++moveAssignCnt;
    return *this;
  }
  ~CtorDtorCounter() { ++dtorCnt; }

  inline static std::size_t ctorCnt = 0;
  inline static std::size_t defaultCtorCnt = 0;
  inline static std::size_t copyCtorCnt = 0;
  inline static std::size_t moveCtorCnt = 0;
  inline static std::size_t dtorCnt = 0;
  inline static std::size_t copyAssignCnt = 0;
  inline static std::size_t moveAssignCnt = 0;
};

TEST_CASE("static_vector constructors", "[static_vector]") {
  constexpr static std::size_t CAPACITY = 10;
  constexpr static int INIT_VALUE = 1;
  const std::initializer_list<int> initList{1, 2, 3, 4, 5};

  SECTION("default") {
    const StaticVector<int, CAPACITY> vector{};

    REQUIRE(vector.size() == 0ull);
    REQUIRE(vector.empty());
    REQUIRE(vector.max_size() == CAPACITY);
    REQUIRE(vector.capacity() == CAPACITY);
  }
  SECTION("default does no unnecessary constructions") {
    {
      const StaticVector<CtorDtorCounter<0>, CAPACITY> vec1{};
    }
    REQUIRE(CtorDtorCounter<0>::defaultCtorCnt == 0);
    REQUIRE(CtorDtorCounter<0>::moveCtorCnt == 0);
    REQUIRE(CtorDtorCounter<0>::copyCtorCnt == 0);
    REQUIRE(CtorDtorCounter<0>::ctorCnt == 0);
    REQUIRE(CtorDtorCounter<0>::dtorCnt == 0);
  }
  SECTION("sized") {
    constexpr static std::size_t SIZE = 5;
    const StaticVector<int, CAPACITY> vector(SIZE);

    REQUIRE(vector.size() == SIZE);
    REQUIRE(!vector.empty());
    REQUIRE(vector.max_size() == CAPACITY);
    REQUIRE(vector.capacity() == CAPACITY);
  }
  SECTION("sized does no unnecessary constructions") {
    constexpr static std::size_t SIZE = 5;
    { const StaticVector<CtorDtorCounter<1>, CAPACITY> vec1(SIZE); }
    REQUIRE(CtorDtorCounter<1>::defaultCtorCnt == SIZE);
    REQUIRE(CtorDtorCounter<1>::moveCtorCnt == 0);
    REQUIRE(CtorDtorCounter<1>::copyCtorCnt == 0);
    REQUIRE(CtorDtorCounter<1>::ctorCnt == SIZE);
    REQUIRE(CtorDtorCounter<1>::dtorCnt == SIZE);
  }
  SECTION("sized init value") {
    constexpr static std::size_t SIZE = 5;
    const StaticVector<int, CAPACITY> vector(SIZE, INIT_VALUE);

    REQUIRE(vector.size() == SIZE);
    REQUIRE(!vector.empty());
    REQUIRE(vector.max_size() == CAPACITY);
    REQUIRE(vector.capacity() == CAPACITY);
    for (std::size_t i = 0; i < vector.size(); ++i) { REQUIRE(vector[i] == INIT_VALUE); }
  }
  SECTION("sized init value does no unnecessary constructions") {
    constexpr static std::size_t SIZE = 5;
    { const StaticVector<CtorDtorCounter<2>, CAPACITY> vector(SIZE, INIT_VALUE); }
    REQUIRE(CtorDtorCounter<2>::defaultCtorCnt == 0);
    REQUIRE(CtorDtorCounter<2>::moveCtorCnt == 0);
    REQUIRE(CtorDtorCounter<2>::copyCtorCnt == SIZE);
    REQUIRE(CtorDtorCounter<2>::ctorCnt == SIZE + 1);// additional ctor call for the init value
    REQUIRE(CtorDtorCounter<2>::dtorCnt == SIZE + 1);// additional dtor call for the init value
  }
  SECTION("iterator") {
    const StaticVector<int, CAPACITY> vector(initList.begin(), initList.end());

    REQUIRE(vector.size() == initList.size());
    REQUIRE(!vector.empty());
    REQUIRE(vector.max_size() == CAPACITY);
    REQUIRE(vector.capacity() == CAPACITY);

    const auto initListIter = initList.begin();
    for (std::size_t i = 0; i < vector.size(); ++i) { REQUIRE(vector[i] == *std::next(initListIter, i)); }
  }
  SECTION("iterator does no unnecessary constructions") {
    const std::initializer_list<CtorDtorCounter<3>> initList{1, 2, 3, 4, 5};
    { const StaticVector<CtorDtorCounter<3>, CAPACITY> vector(initList.begin(), initList.end()); }
    REQUIRE(CtorDtorCounter<3>::defaultCtorCnt == 0);
    REQUIRE(CtorDtorCounter<3>::moveCtorCnt == 0);
    REQUIRE(CtorDtorCounter<3>::copyCtorCnt == initList.size());
    REQUIRE(CtorDtorCounter<3>::ctorCnt == initList.size() * 2);// *2 because initializer_list construction
    REQUIRE(CtorDtorCounter<3>::dtorCnt == initList.size());    // initializer_list is not yet destroyed, so no *2
  }
  SECTION("initializer_list") {
    const StaticVector<int, CAPACITY> vector{initList};

    REQUIRE(vector.size() == initList.size());
    REQUIRE(!vector.empty());
    REQUIRE(vector.max_size() == CAPACITY);
    REQUIRE(vector.capacity() == CAPACITY);

    const auto initListIter = initList.begin();
    for (std::size_t i = 0; i < vector.size(); ++i) { REQUIRE(vector[i] == *std::next(initListIter, i)); }
  }
  SECTION("initializer_list does no unnecessary constructions") {
    constexpr static std::size_t SIZE = 5;
    { const StaticVector<CtorDtorCounter<3>, CAPACITY> vector{1, 2, 3, 4, 5}; }
    REQUIRE(CtorDtorCounter<3>::defaultCtorCnt == 0);
    REQUIRE(CtorDtorCounter<3>::moveCtorCnt == 0);       // init list sucks and can't be moved from
    REQUIRE(CtorDtorCounter<3>::copyCtorCnt == SIZE * 2);// *2 because initializer_list copy
    REQUIRE(CtorDtorCounter<3>::ctorCnt == SIZE * 4);    // *4 because initializer_list construction and its copy
    REQUIRE(CtorDtorCounter<3>::dtorCnt == SIZE * 4);    // *4 because initializer_list construction
  }
  SECTION("copy") {
    const StaticVector<int, CAPACITY> src{initList};
    const StaticVector<int, CAPACITY> vector{src};

    REQUIRE(vector.size() == src.size());
    REQUIRE(!vector.empty());
    REQUIRE(vector.max_size() == CAPACITY);
    REQUIRE(vector.capacity() == CAPACITY);

    for (std::size_t i = 0; i < vector.size(); ++i) { REQUIRE(vector[i] == src[i]); }
  }
  SECTION("copy does no unnecessary constructions") {
    constexpr static std::size_t SIZE = 5;
    {
      const StaticVector<CtorDtorCounter<4>, CAPACITY> src(SIZE);
      const StaticVector<CtorDtorCounter<4>, CAPACITY> vector = StaticVector<CtorDtorCounter<4>, CAPACITY>(src);
    }
    REQUIRE(CtorDtorCounter<4>::defaultCtorCnt == SIZE);// source construction
    REQUIRE(CtorDtorCounter<4>::moveCtorCnt == 0);
    REQUIRE(CtorDtorCounter<4>::copyCtorCnt == SIZE);
    REQUIRE(CtorDtorCounter<4>::ctorCnt == SIZE * 2);// *2 because source construction
    REQUIRE(CtorDtorCounter<4>::dtorCnt == SIZE * 2);// *2 because source destruction
  }
  SECTION("move") {
    StaticVector<int, CAPACITY> src{initList};
    const StaticVector<int, CAPACITY> vector{std::move(src)};

    REQUIRE(vector.size() == initList.size());
    REQUIRE(!vector.empty());
    REQUIRE(vector.max_size() == CAPACITY);
    REQUIRE(vector.capacity() == CAPACITY);

    const auto initListIter = initList.begin();
    for (std::size_t i = 0; i < vector.size(); ++i) { REQUIRE(vector[i] == *std::next(initListIter, i)); }
  }
  SECTION("move does no unnecessary constructions") {
    constexpr static std::size_t SIZE = 5;
    {
      StaticVector<CtorDtorCounter<5>, CAPACITY> src(SIZE);
      const StaticVector<CtorDtorCounter<5>, CAPACITY> vector = StaticVector<CtorDtorCounter<5>, CAPACITY>(std::move(src));
    }
    REQUIRE(CtorDtorCounter<5>::defaultCtorCnt == SIZE);// source construction
    REQUIRE(CtorDtorCounter<5>::moveCtorCnt == SIZE);
    REQUIRE(CtorDtorCounter<5>::copyCtorCnt == 0);
    REQUIRE(CtorDtorCounter<5>::ctorCnt == SIZE * 2);// *2 because source construction
    REQUIRE(CtorDtorCounter<5>::dtorCnt == SIZE * 2);// *2 because source destruction
  }
}

TEST_CASE("static_vector operators", "[static_vector]") {
  constexpr static std::size_t CAPACITY = 10;

  SECTION("equality is detected correctly <=>") {

    const StaticVector<ThreeWayComparable, CAPACITY> vec1{ThreeWayComparable{1}, ThreeWayComparable{2},
                                                           ThreeWayComparable{3}, ThreeWayComparable{4},
                                                           ThreeWayComparable{5}};
    const StaticVector<ThreeWayComparable, CAPACITY> vec2 = vec1;

    REQUIRE(vec1 == vec2);
    REQUIRE(!(vec1 != vec2));
  }

  SECTION("equality is detected correctly == !=") {

    const StaticVector<OldSchoolComparable, CAPACITY> vec1{OldSchoolComparable{1}, OldSchoolComparable{2},
                                                            OldSchoolComparable{3}, OldSchoolComparable{4},
                                                            OldSchoolComparable{5}};
    const StaticVector<OldSchoolComparable, CAPACITY> vec2 = vec1;

    REQUIRE(vec1 == vec2);
    REQUIRE(!(vec1 != vec2));
  }

  SECTION("inequality is detected correctly <=>") {

    const StaticVector<ThreeWayComparable, CAPACITY> vec1{ThreeWayComparable{1}, ThreeWayComparable{2},
                                                           ThreeWayComparable{3}, ThreeWayComparable{4},
                                                           ThreeWayComparable{5}};
    const StaticVector<ThreeWayComparable, CAPACITY> vec2{ThreeWayComparable{1}, ThreeWayComparable{2},
                                                           ThreeWayComparable{3}, ThreeWayComparable{4}};

    REQUIRE(vec1 != vec2);
    REQUIRE(!(vec1 == vec2));
  }

  SECTION("inequality is detected correctly == !=") {

    const StaticVector<OldSchoolComparable, CAPACITY> vec1{OldSchoolComparable{1}, OldSchoolComparable{2},
                                                            OldSchoolComparable{3}, OldSchoolComparable{4},
                                                            OldSchoolComparable{5}};
    const StaticVector<OldSchoolComparable, CAPACITY> vec2{OldSchoolComparable{1}, OldSchoolComparable{2},
                                                            OldSchoolComparable{3}, OldSchoolComparable{4}};

    REQUIRE(vec1 != vec2);
    REQUIRE(!(vec1 == vec2));
  }

  SECTION("greater is detected correctly <=>") {

    const StaticVector<ThreeWayComparable, CAPACITY> vec1{ThreeWayComparable{5}, ThreeWayComparable{2},
                                                           ThreeWayComparable{3}, ThreeWayComparable{4},
                                                           ThreeWayComparable{5}};
    const StaticVector<ThreeWayComparable, CAPACITY> vec2{ThreeWayComparable{1}, ThreeWayComparable{2},
                                                           ThreeWayComparable{3}, ThreeWayComparable{4}};

    REQUIRE(vec1 > vec2);
    REQUIRE(!(vec1 < vec2));
  }

  SECTION("greater is detected correctly >") {

    const StaticVector<OldSchoolComparable, CAPACITY> vec1{OldSchoolComparable{5}, OldSchoolComparable{2},
                                                            OldSchoolComparable{3}, OldSchoolComparable{4},
                                                            OldSchoolComparable{5}};
    const StaticVector<OldSchoolComparable, CAPACITY> vec2{OldSchoolComparable{1}, OldSchoolComparable{2},
                                                            OldSchoolComparable{3}, OldSchoolComparable{4}};

    REQUIRE(vec1 > vec2);
    REQUIRE(!(vec1 < vec2));
  }

  SECTION("greater or equal is detected correctly <=>") {

    const StaticVector<ThreeWayComparable, CAPACITY> vec1{ThreeWayComparable{5}, ThreeWayComparable{2},
                                                           ThreeWayComparable{3}, ThreeWayComparable{4},
                                                           ThreeWayComparable{5}};
    const StaticVector<ThreeWayComparable, CAPACITY> vec2{ThreeWayComparable{1}, ThreeWayComparable{2},
                                                           ThreeWayComparable{3}, ThreeWayComparable{4}};
    const auto vec3 = vec1;

    REQUIRE(vec1 >= vec2);
    REQUIRE(!(vec1 < vec2));

    REQUIRE(vec1 >= vec3);
  }

  SECTION("greater or equal is detected correctly >=") {

    const StaticVector<OldSchoolComparable, CAPACITY> vec1{OldSchoolComparable{5}, OldSchoolComparable{2},
                                                            OldSchoolComparable{3}, OldSchoolComparable{4},
                                                            OldSchoolComparable{5}};
    const StaticVector<OldSchoolComparable, CAPACITY> vec2{OldSchoolComparable{1}, OldSchoolComparable{2},
                                                            OldSchoolComparable{3}, OldSchoolComparable{4}};
    const auto vec3 = vec1;

    REQUIRE(vec1 > vec2);
    REQUIRE(!(vec1 < vec2));
    REQUIRE(vec1 >= vec3);
  }

  SECTION("less is detected correctly <=>") {

    const StaticVector<ThreeWayComparable, CAPACITY> vec1{ThreeWayComparable{1}, ThreeWayComparable{2},
                                                           ThreeWayComparable{3}, ThreeWayComparable{4},
                                                           ThreeWayComparable{5}};
    const StaticVector<ThreeWayComparable, CAPACITY> vec2{ThreeWayComparable{5}, ThreeWayComparable{2},
                                                           ThreeWayComparable{3}, ThreeWayComparable{4}};

    REQUIRE(vec1 < vec2);
    REQUIRE(!(vec1 > vec2));
  }

  SECTION("less is detected correctly >") {

    const StaticVector<OldSchoolComparable, CAPACITY> vec1{OldSchoolComparable{1}, OldSchoolComparable{2},
                                                            OldSchoolComparable{3}, OldSchoolComparable{4},
                                                            OldSchoolComparable{5}};
    const StaticVector<OldSchoolComparable, CAPACITY> vec2{OldSchoolComparable{5}, OldSchoolComparable{2},
                                                            OldSchoolComparable{3}, OldSchoolComparable{4}};

    REQUIRE(vec1 < vec2);
    REQUIRE(!(vec1 > vec2));
  }

  SECTION("less or equal is detected correctly <=>") {

    const StaticVector<ThreeWayComparable, CAPACITY> vec1{ThreeWayComparable{1}, ThreeWayComparable{2},
                                                           ThreeWayComparable{3}, ThreeWayComparable{4},
                                                           ThreeWayComparable{5}};
    const StaticVector<ThreeWayComparable, CAPACITY> vec2{ThreeWayComparable{5}, ThreeWayComparable{2},
                                                           ThreeWayComparable{3}, ThreeWayComparable{4}};
    const auto vec3 = vec1;

    REQUIRE(vec1 <= vec2);
    REQUIRE(!(vec1 > vec2));

    REQUIRE(vec1 <= vec3);
  }

  SECTION("less or equal is detected correctly >=") {

    const StaticVector<OldSchoolComparable, CAPACITY> vec1{OldSchoolComparable{1}, OldSchoolComparable{2},
                                                            OldSchoolComparable{3}, OldSchoolComparable{4},
                                                            OldSchoolComparable{5}};
    const StaticVector<OldSchoolComparable, CAPACITY> vec2{OldSchoolComparable{5}, OldSchoolComparable{2},
                                                            OldSchoolComparable{3}, OldSchoolComparable{4}};
    const auto vec3 = vec1;

    REQUIRE(vec1 < vec2);
    REQUIRE(!(vec1 > vec2));
    REQUIRE(vec1 <= vec3);
  }

  SECTION("copy assignment") {
    const StaticVector<int, CAPACITY> vec1{1, 2, 3, 4, 5};
    const StaticVector<int, CAPACITY> vec2{5, 2, 3, 4};

    StaticVector<int, CAPACITY> vec3{};
    vec3 = vec1;
    REQUIRE(vec3.size() == vec1.size());
    for (std::size_t i = 0; i < vec1.size(); ++i) { REQUIRE(vec1[i] == vec3[i]); }

    vec3 = vec2;
    REQUIRE(vec3.size() == vec2.size());
    for (std::size_t i = 0; i < vec2.size(); ++i) { REQUIRE(vec2[i] == vec3[i]); }
  }

  SECTION("copy assignment does no unnecessary construction when copying to empty") {
    {
      const StaticVector<CtorDtorCounter<6>, CAPACITY> vec1(5);
      StaticVector<CtorDtorCounter<6>, CAPACITY> vec3{};
      vec3 = vec1;
    }

    REQUIRE(CtorDtorCounter<6>::defaultCtorCnt == 5);
    REQUIRE(CtorDtorCounter<6>::moveCtorCnt == 0);
    REQUIRE(CtorDtorCounter<6>::moveAssignCnt == 0);
    REQUIRE(CtorDtorCounter<6>::copyCtorCnt == 5);
    REQUIRE(CtorDtorCounter<6>::copyAssignCnt == 0);
    REQUIRE(CtorDtorCounter<6>::ctorCnt == 5 * 2);// *2 because source construction
    REQUIRE(CtorDtorCounter<6>::dtorCnt == 5 * 2);// *2 because source destruction
  }

  SECTION("copy assignment does no unnecessary construction when copying to smaller") {
    {
      const StaticVector<CtorDtorCounter<7>, CAPACITY> vec1(5);
      StaticVector<CtorDtorCounter<7>, CAPACITY> vec3(2);
      vec3 = vec1;
    }

    REQUIRE(CtorDtorCounter<7>::defaultCtorCnt == 2 + 5);// default init in vec3 and vec1
    REQUIRE(CtorDtorCounter<7>::moveCtorCnt == 0);
    REQUIRE(CtorDtorCounter<7>::moveAssignCnt == 0);
    REQUIRE(CtorDtorCounter<7>::copyCtorCnt == 3);    // 3 for >size()
    REQUIRE(CtorDtorCounter<7>::copyAssignCnt == 2);  // 2 elements already exist
    REQUIRE(CtorDtorCounter<7>::ctorCnt == 5 + 2 + 3);// 5 because source construction, 2 for vec3, 3 for vec1 copies
    REQUIRE(CtorDtorCounter<7>::dtorCnt == 5 * 2);    // *2 because source destruction
  }

  SECTION("copy assignment does no unnecessary construction when copying to bigger") {
    {
      const StaticVector<CtorDtorCounter<8>, CAPACITY> vec1(2);
      StaticVector<CtorDtorCounter<8>, CAPACITY> vec3(5);
      vec3 = vec1;
      REQUIRE(CtorDtorCounter<8>::dtorCnt == 3);// we expect 3 elements to be destroyed since the source is smaller
    }

    REQUIRE(CtorDtorCounter<8>::defaultCtorCnt == 5 + 2);// 5 vec3, 2 vec1
    REQUIRE(CtorDtorCounter<8>::moveCtorCnt == 0);
    REQUIRE(CtorDtorCounter<8>::moveAssignCnt == 0);
    REQUIRE(CtorDtorCounter<8>::copyCtorCnt == 0);
    REQUIRE(CtorDtorCounter<8>::copyAssignCnt == 2);
    REQUIRE(CtorDtorCounter<8>::ctorCnt == 2 + 5);// 2 because source construction, 5 for vec3
    REQUIRE(CtorDtorCounter<8>::dtorCnt
            == 2 + 3 + 2);// 2 source, 3 for initially destroyed during assignment and 2 remaining
  }
}
