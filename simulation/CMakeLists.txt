# Все .cpp из simulation/src собираются в библиотеку физики.
file(GLOB SIM_SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_library(gaz_core ${SIM_SOURCES})

# PUBLIC: кто подключает gaz_core, автоматически получает simulation/include.
target_include_directories(gaz_core PUBLIC include)
target_compile_options(gaz_core PRIVATE -Wall -Wextra -Wpedantic)