# Все .cpp из visual/src (включая main.cpp визуализации).
file(GLOB VIS_SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(gaz_visual ${VIS_SOURCES})

# PRIVATE: заголовки визуализации нужны только ей самой.
target_include_directories(gaz_visual PRIVATE include)
target_compile_options(gaz_visual PRIVATE -Wall -Wextra)
target_link_libraries(gaz_visual PRIVATE gaz_core sfml-graphics sfml-window sfml-system)

# cmake --build build --target run_visual  — собрать и запустить.
add_custom_target(run_visual
    COMMAND gaz_visual
    DEPENDS gaz_visual
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})