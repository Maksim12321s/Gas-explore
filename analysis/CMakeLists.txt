file(GLOB ANALYSIS_SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_library(gaz_analysis ${ANALYSIS_SOURCES})
target_include_directories(gaz_analysis PUBLIC include)
target_link_libraries(gaz_analysis PUBLIC gaz_core)