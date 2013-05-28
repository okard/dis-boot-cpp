

cmake_minimum_required(VERSION 2.8.10)

project(disc)

# CUL
add_subdirectory("deps/cul")
include_directories("deps/cul/include")

# C++11
SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11")

# libplf
add_subdirectory("libplf")
# libdis
add_subdirectory("libdis")

# disc
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_subdirectory("disc")
