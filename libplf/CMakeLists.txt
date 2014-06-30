# cmake file for the programming language framework (plf)
cmake_minimum_required(VERSION 2.8.10)

# plf library
file(GLOB_RECURSE LIBPLF_SOURCES src/*.cpp src/*.hpp include/*.hpp)
include_directories("include")

SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/lib)
add_library(plf STATIC ${LIBPLF_SOURCES})
target_link_libraries(plf culio)

# tests
file(GLOB_RECURSE LIBPLF_TESTS tests/*.cpp)
add_executable(libplf_tests ${LIBPLF_TESTS})
target_link_libraries(libplf_tests plf)
add_custom_target(check_libplf COMMAND libplf_tests DEPENDS libplf_tests)

