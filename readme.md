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
	
* libplf-c 		- C Code Generation/Compiling
* libplf-llvm 	- LLVM Code Generation/Compiling
* libplf-js		- Javascript Code Generation
	
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
	- Including STL
* CMake
* make or ninja
* CUL - C++ Utility Library (included as subrepository)


## Build on Linux/Posix

	$ ./build.sh

## Build on Windows

	TODO

# TODOs

Only specific to cpp stuff other todos see dis/todo.md (dis repo)

* Cleanup Buffers
	* MutableBuffer
	* ConstBuffer
* Implement a RingBuffer for Tokens
	* which can be used by parser for token lookups
* Fix Include Guards
