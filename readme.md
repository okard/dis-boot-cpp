# Bootstrap Compiler for Dis


Written in C++11
License: MIT


# Layout

* libplf - programming language framework
	- SourceManager
	- AST
	- Semantic Analysis
	- Code Gen (C/JS/LLVM)
	- System/Platform Abstraction
	
* libplf-c - C Code Generation/Compiling
* libplf-llvm -  LLVM Code Generation/Compiling
	
* libdis - dis programming language library
	- Handle all syntax stuff
	- Token
	- Lexer
	- Parser
	- Printer
	
* disc - dis compiler binary
	- use libdis & libplf
	- compiler command line frontend

# Build
	
## Dependencies

* C++11 Compiler
* CMake
* make or ninja
* CUL - C++ Utility Library (included as subrepository)


## Build on Linux/Posix

	$ ./build.sh

## Build on Windows

	TODO

# TODOs

* Cleanup Buffers
* Implement a RingBuffer for Tokens
