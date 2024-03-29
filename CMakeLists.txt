cmake_minimum_required(VERSION 3.16)

project(pf_common
        VERSION 1.3.2
        DESCRIPTION "Header only library with concepts and other commonly used things"
        HOMEPAGE_URL "https://github.com/PetrFlajsingr/pf_common"
        )


option(PF_COMMON_TESTS "Enable test & download of test framework" OFF)

if (MSVC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.29.30129 AND CMAKE_VERSION VERSION_GREATER 3.20.3)
    set(CMAKE_CXX_STANDARD 23) # /std:c++latest - unlocks the non stable cpp20 features. For new 16.11 versions
else ()
    set(CMAKE_CXX_STANDARD 20) # /std:c++latest for msvc and -std=c++20 for everyone else.
endif ()

option(BUILD_DOCS "build doxygen docs" OFF)

find_package(magic_enum CONFIG REQUIRED)

include(GNUInstallDirs)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(
        ${PROJECT_NAME}
        INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)
target_link_libraries(${PROJECT_NAME} INTERFACE magic_enum::magic_enum)

install(TARGETS ${PROJECT_NAME}
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
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        list(APPEND flags "/std:c++latest" "/Wall")
    else ()
       list(APPEND flags "-fconcepts" "-fconcepts-diagnostics-depth=10" "-Werror=return-type" "-fcoroutines"
               "-Wall" "-Wextra" "-Werror" "-Wpedantic" "-Wno-unknown-pragmas" "-Wno-unused-function"
               "-Wpointer-arith" "-Wno-cast-qual" "-Wno-type-limits" "-fno-strict-aliasing")
    endif ()
    find_package(Catch2 CONFIG REQUIRED)

    enable_testing()
    include_directories(include)
    add_executable(pf_common_tests tests/LazyInit.cpp tests/ScopeExit.cpp
            tests/ByteLiterals.cpp tests/algorithm.cpp tests/StaticVector.cpp tests/SmallVector.cpp
            tests/concepts.cpp tests/enums.cpp tests/array.cpp tests/StringLiteral.cpp tests/uuid.cpp tests/casts.cpp)

    target_link_libraries(pf_common_tests PRIVATE pf_common::pf_common Catch2::Catch2WithMain)
    target_compile_options(pf_common_tests PRIVATE ${flags})

    include(CTest)
    include(Catch)
    include(ParseAndAddCatchTests)
    ParseAndAddCatchTests(pf_common_tests)
endif ()

if (BUILD_DOCS)
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        doxygen_add_docs(pf_common_doc ALL
                ${CMAKE_SOURCE_DIR}/include/pf_common
                )
    endif ()
endif ()