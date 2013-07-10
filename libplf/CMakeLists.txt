# cmake file for the programming language framework (plf)
cmake_minimum_required(VERSION 2.8.10)

# plf library
file(GLOB_RECURSE LIBPLF_SOURCES src/*.cpp)
include_directories("include")

SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/lib)
add_library(plf STATIC ${LIBPLF_SOURCES})
target_link_libraries(plf culio)

