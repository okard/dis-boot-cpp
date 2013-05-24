# cmake file for the dis compiler library 
cmake_minimum_required(VERSION 2.8.10)

# dis compiler library
file(GLOB_RECURSE LIBDIS_SOURCES src/*.cpp)
include_directories("include")
include_directories("../libplf/include")

add_library(dis STATIC ${LIBDIS_SOURCES})
target_link_libraries(dis plf)

