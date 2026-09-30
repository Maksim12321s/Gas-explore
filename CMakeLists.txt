cmake_minimum_required(VERSION 3.14)
project(gaz LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ---------- Физика: библиотека gaz_core ----------
add_subdirectory(simulation)

# ---------- Консольная проверка ----------
add_executable(gaz console/main.cpp)
target_link_libraries(gaz PRIVATE gaz_core)

# ---------- Визуализация: только если есть SFML ----------
find_package(SFML 2.5 COMPONENTS graphics window system QUIET)
if(SFML_FOUND)
    add_subdirectory(visual)
else()
    message(STATUS "SFML not found: gaz_visual will not be built")
endif()

# ---------- Тесты на GoogleTest ----------
option(GAZ_BUILD_TESTS "Build unit tests" ON)
if(GAZ_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
    )
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    file(GLOB TEST_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/tests/*.cpp)
    if(TEST_SOURCES)
        add_executable(gaz_tests ${TEST_SOURCES})
        target_link_libraries(gaz_tests PRIVATE gaz_core GTest::gtest_main)
        include(GoogleTest)
        gtest_discover_tests(gaz_tests)
    endif()
endif()