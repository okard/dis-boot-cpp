cmake_minimum_required(VERSION 2.8.10)


# dis library
file(GLOB_RECURSE DISC_SOURCES src/*.hpp src/*.cpp)
include_directories("../libplf/include")
include_directories("../libdis/include")
add_executable(disc ${DISC_SOURCES})
target_link_libraries(disc cullog plf plfllvm dis)
#set_target_properties(disc PROPERTIES COMPILE_FLAGS "-flto")
