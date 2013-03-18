cmake_minimum_required(VERSION 2.8.10)


# dis library
file(GLOB_RECURSE DISC_SOURCES src/*.cpp)
add_executable(disc ${DISC_SOURCES})
target_link_libraries(disc cullog plf dis)
