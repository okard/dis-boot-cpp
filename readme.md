# Bootstrap Compiler for Dis


Written in C++11
License: MIT


# Layout

* libplf - programming language framework
	- SourceManager
	- AST
	- Semantic Analysis
	- Code Gen (C/JS)
	
* libdis - dis programming language library
	- Token
	- Lexer
	- Parser
	
* disc - dis compiler binary
	- use libdis & libplf
	- compiler command line frontend

# Build
	
## Dependencies

* C++11 Compiler
* CMake
* make or ninja


## Build on Linux/Posix

	$ ./build.sh

## Build on Windows

TODO

