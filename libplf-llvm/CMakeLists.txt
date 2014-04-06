# cmake file for the programming language framework (plf)
cmake_minimum_required(VERSION 2.8.10)


#
#llvm-config --cppflags --ldflags --libs core
#-I/usr/include  -DNDEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
#-L/usr/lib  -lz -lpthread -lffi -lcurses -ldl -lm
#-lLLVMCore -lLLVMSupport

# LLVM Stuff
add_definitions(-D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS)

# plf library
file(GLOB_RECURSE LIBPLFLLVM_SOURCES src/*.cpp src/*.hpp include/*.hpp)
include_directories("include")
include_directories("../libplf/include")


SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/lib)
add_library(plfllvm STATIC ${LIBPLFLLVM_SOURCES})
target_link_libraries(plfllvm plf "LLVM-3.4")
