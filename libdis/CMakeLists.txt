# cmake file for the dis compiler library 
cmake_minimum_required(VERSION 2.8.10)

# dis compiler library
file(GLOB_RECURSE LIBDIS_SOURCES src/*.cpp include/*.hpp)
include_directories("include")
include_directories("../libplf/include")

SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/lib)
add_library(dis STATIC ${LIBDIS_SOURCES})
target_link_libraries(dis plf)


