
cmake_minimum_required(VERSION 2.8.10)

project(disc)
enable_testing()

# CUL
add_subdirectory("deps/cul")
include_directories("deps/cul/include")

# C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

# -flto

# libplf
add_subdirectory("libplf")
add_subdirectory("libplf-llvm")

# libdis
add_subdirectory("libdis")

# disc
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_subdirectory("disc")
