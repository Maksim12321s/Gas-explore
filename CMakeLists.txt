cmake_minimum_required(VERSION 3.14)
project(gaz LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Если тип сборки не указан, собираем с оптимизацией.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Библиотека с физикой: её используют программа, визуализация и тесты.
add_library(gaz_core src/simulation.cpp)
target_include_directories(gaz_core PUBLIC include)
target_compile_options(gaz_core PUBLIC -Wall -Wextra -Wpedantic)

# Консольная программа.
add_executable(gaz main.cpp)
target_link_libraries(gaz PRIVATE gaz_core)

# Визуализация: собирается, только если SFML установлена.
find_package(SFML 2.5 COMPONENTS graphics window system QUIET)
if(SFML_FOUND)
    add_executable(gaz_visual visual.cpp)
    target_link_libraries(gaz_visual PRIVATE gaz_core sfml-graphics sfml-window sfml-system)
else()
    message(STATUS "SFML not found: gaz_visual will not be built")
endif()

# Тесты: все .cpp из папки tests собираются в одну программу (в них должен быть один main).
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp)
if(TEST_SOURCES)
    enable_testing()
    add_executable(gaz_tests ${TEST_SOURCES})
    target_link_libraries(gaz_tests PRIVATE gaz_core)
    add_test(NAME gaz_tests COMMAND gaz_tests)
endif()