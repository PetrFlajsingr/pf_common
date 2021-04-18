cmake_minimum_required(VERSION 3.16)

include("cmake/HunterGate.cmake")
HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.269.tar.gz"
        SHA1 "64024b7b95b4c86d50ae05b926814448c93a70a0"
)

project(pf_common
        VERSION 1.0.5
        DESCRIPTION "Header only library with concepts and other commonly used things"
        HOMEPAGE_URL "https://github.com/PetrFlajsingr/pf_common"
        )

list(APPEND flags "-fconcepts" "-fconcepts-diagnostics-depth=10" "-Werror=return-type" "-fcoroutines"
        "-Wall" "-Wextra" "-Werror" "-Wpedantic" "-Wno-unknown-pragmas" "-Wno-unused-function"
        "-Wpointer-arith" "-Wno-cast-qual" "-Wno-type-limits" "-fno-strict-aliasing")

option(PF_COMMON_TESTS "Enable test & download of test framework" )

set(CMAKE_CXX_STANDARD 20)

set(CPM_DOWNLOAD_LOCATION "${CMAKE_SOURCE_DIR}/cmake/CPM.cmake")
set(CPM_DOWNLOAD_VERSION 0.27.4)

if (NOT (EXISTS ${CPM_DOWNLOAD_LOCATION} AND CPM_VERSION STREQUAL CPM_DOWNLOAD_VERSION))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif ()

include(${CPM_DOWNLOAD_LOCATION})

option(ENABLE_STACKTRACE "enable stacktrace in exceptions" OFF)
option(BUILD_DOCS "build doxygen docs" OFF)

if (ENABLE_STACKTRACE)
    add_compile_definitions(STACKTRACE_ENABLE)
endif()

hunter_add_package(fmt)
find_package(fmt CONFIG REQUIRED)
hunter_add_package(glm)
find_package(glm CONFIG REQUIRED)
CPMAddPackage(
        NAME cppcoro
        GITHUB_REPOSITORY andreasbuhr/cppcoro
        GIT_TAG master
)
CPMAddPackage(
        NAME backward_cpp
        GITHUB_REPOSITORY bombela/backward-cpp
        GIT_TAG v1.5
)
set(RANGE_V3_TESTS OFF)
CPMAddPackage(
        NAME range-v3
        GITHUB_REPOSITORY ericniebler/range-v3
        GIT_TAG 0.11.0
)
CPMAddPackage(
        NAME magic_enum
        GITHUB_REPOSITORY Neargye/magic_enum
        GIT_TAG v0.6.6
)


if (PF_COMMON_TESTS)
    message("Downloading Catch2")
    hunter_add_package(Catch)
    find_package(Catch2 CONFIG REQUIRED)
endif()

include(GNUInstallDirs)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(
        ${PROJECT_NAME}
        INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)
target_link_libraries(${PROJECT_NAME} INTERFACE cppcoro magic_enum ${BACKWARD_ENABLE} range-v3 fmt::fmt)

install(TARGETS ${PROJECT_NAME} magic_enum
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})


include(CMakePackageConfigHelpers)

write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)


install(EXPORT ${PROJECT_NAME}_Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/pf_common DESTINATION include)

set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")

include(CPack)

if (PF_COMMON_TESTS)
    enable_testing()
    include_directories(include)
    add_executable(pf_common_tests tests/test_main.cpp tests/LazyInit.cpp tests/RAII.cpp
            tests/ByteLiterals.cpp tests/algorithm.cpp tests/ObjectPool.cpp tests/coroutines.cpp tests/concepts.cpp tests/enums.cpp ${BACKWARD_ENABLE})
    target_link_libraries(pf_common_tests PRIVATE magic_enum Catch2::Catch2 cppcoro range-v3 fmt::fmt)
    target_compile_options(pf_common_tests PRIVATE ${flags})
    add_backward(pf_common_tests)

    include(CTest)
    include(Catch)
    include(ParseAndAddCatchTests)
    ParseAndAddCatchTests(pf_common_tests)
endif()

if (BUILD_DOCS)
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        doxygen_add_docs(pf_common_doc ALL
                ${CMAKE_SOURCE_DIR}/include/pf_common
                )
    endif()
endif()