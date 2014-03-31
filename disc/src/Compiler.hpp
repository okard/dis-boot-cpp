/*
Dis Compiler

Copyright (c) 2013 okard

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/



#include <plf/base/External.hpp>
#include <plf/base/Buffer.hpp>
#include <plf/base/SourceFile.hpp>
#include <plf/base/SourceManager.hpp>

#include <plf/ast/Crate.hpp>

#include <plf/gen/CodeGen.hpp>
#include <plf/gen/Linker.hpp>
#include <plf/gen/Target.hpp>

#include <dis/Lexer.hpp>
#include <dis/Parser.hpp>

//TODO split into a base compiler class as example for lib usage of libdis and libplf


//Only one CompilationUnit with multiple source files?
struct CompilationUnit
{
	//FileName
	plf::SourcePtr sourcePtr;
	plf::Crate crate;

	//ObjectFile obj;

	//Info

	//flags? -> lex/parse/semantic/compile/link
	bool do_lex = false;
	bool do_parse = false;

	//status -> error, parsed, semantic(1..N),...

	CompilationUnit(const plf::SourcePtr& src)
		: sourcePtr(src)
	{
	}
};


/**
 * @brief Compiler driver class
 */
class Compiler
{
private:
	dis::Lexer lexer_;
	dis::Parser parser_;

	plf::List<CompilationUnit> units_;

	//custom source manager?
	
	//source files to compile
	//save sourceid/filename here?
	
	
	//struct module(filename,NodePtr, state)
	
	// already opened files
	// std::map<filename, SourceId> //filename to Source id
	
	// already parsed/files?
	// std::map<SourceId, NodePtr> //source id to ast
	
public:
	Compiler();
	~Compiler();
	
	//void parse_args()
	int run(int argc, char *argv[]);
	

	bool parse();
	bool semantic();
	bool compile();		//requires process call
	bool link();	  //requires process call
	
	//import search file handling
	//TODO requires directory/path
	//->handling via Semantic Interface? with callbacks?
	//void resolveImports();
	//void checkPaths();
	
	
	void testLex(const char* filename);
	void testParse(const char* filename);
};
