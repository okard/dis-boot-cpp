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

#include "Compiler.hpp"

#include <cstring>
#include <iostream>



#include <plf/ast/Crate.hpp>
#include <plf/ast/Declaration.hpp>

#include <dis/Printer.hpp>

using namespace plf;
using namespace dis;


struct CompilationUnit
{
	SourceId sourceId;
	plf::Crate crate;
	//SymbolTable? -> in crate
	//ObjectFile obj;
};

/*
 { "-parse", 1, std::function<void(argc, argv)> } 
 rest


 Command Line Options:

Generic Target Information
	-arch		arm, x32, x64
	-platform	Linux, Posix, Windows,FreeBSD, ...
	-backend    llvm, js, jit, etc


Compilation Style:
	-c				to object file
	-executable     to executeable
	-static         to static library
	-shared			to shared library (dll)

	//when -c compile each file in a own crate (append path)

Module Prefixed
	-parser     Parser Options..
	-sem		Semantic
	-cg			Code Generation
	-link		Linker stuff

LLVM Backend:

	-cg-llvm-dump-ir		llvm ir


*/


Compiler::Compiler()
	: parser_(lexer_)
{
	
}

Compiler::~Compiler()
{
	
}

int Compiler::run(int argc, char *argv[])
{
	std::cout << "Running dis compiler" << std::endl;



	
	//- parse arguments
	for(int i=1; i < argc; i++)
	{
		if(strcmp(argv[i], "-lex") == 0 && ((i+1) < argc))
		{
			std::cout << "Lexer Dump:" << std::endl;
			//check if file exists
			testLex(argv[i+1]);
			return 0;
		}
		
		if(strcmp(argv[i], "-parse") == 0 && ((i+1) < argc))
		{
			std::cout << "Parser Dump:" << std::endl;
			//check if file exists
			testParse(argv[i+1]);
			return 0;
		}


		//-arch / -platform / -backend
		//-c / -executable / -static / -shared
		//-llvm dump ir

		//when -c compile each file in a own crate (append path)


		//not an option so it is a source file
		_parsed_sourcefiles.push_back(std::make_shared<Buffer>(argv[i]));
	}





	//semantic runs


	
	//auto src = std::make_shared<SourceFile>();
	//src->open(argv[1]);
	//lexer_.open(src);
	
	
	//0. auto crate = new plf::Crate();

	//1. open files
	//2. parse input files (Lex->Parse->Ast)
		// for(src: sources)
			 //parse; crate->add(decl Node);

	//3. resolve imports (Ast)
		//resolve imports (&crate)
			// root crate "exports" module structure
			// resolve the modules defined by root crate and add sources/asts for modules

	//4. semantic run (Ast -> DAst)
		//semantic(&crate)

	//5. codegen ( Ast -> ObjectFile)
		//codegen(&crate)

	//6. link ( ObjectFile -> Binary)
		//link(&crate);
	
	return 0;
}

void Compiler::parse()
{
	plf::Crate crate;
	for(auto src_file: _parsed_sourcefiles)
	{
		auto src = SourceManager::getInstance().loadFile(src_file->ptr());
		lexer_.open(src);

		auto n = parser_.parse();
		auto decl = n->to<Declaration>(); //Declaration.hpp required because of knowing inheritance
		//add to crate
		crate.decls.push_back(decl);
	}
}

void Compiler::semantic()
{
	//semantic steps

	//-> import solving for mods, use decls

	//-> type solving?

	//-> Run Interpreter for compile time execution

	//-> Optimizer stuff (constant folding?)
}


void Compiler::testLex(const char* filename)
{
	//check if file exists
	
	auto src = SourceManager::getInstance().loadFile(filename);
	
	Lexer lexer;
	lexer.open(src);
	
	Token tok;
	do
	{
		lexer.next(tok);
		std::cout << "Token(L" << tok.loc.line << ", C" << tok.loc.column << "): " << dis::toString(tok.id);

		//when has buffer content
		if(tok.buffer)
		{
			std::cout << " Buffer: ";
			std::cout.write(tok.buffer->ptr(), tok.buffer->size());
		}
		std::cout << std::endl;
	}
	while(tok.id != TokenId::Eof);
}


void Compiler::testParse(const char* filename)
{
	//check if file exists
	
	auto src = std::make_shared<SourceFile>();
	src->open(filename);
	
	Lexer lexer;
	Parser parser(lexer);
	lexer.open(src);
	
	//parse file
	Printer p;
	ParamPtr pp;
	NodePtr n;
	do
	{
		n = parser.parse();
		//print:
		n->accept(p, pp);
	}
	while(n && n->kind != NodeKind::Error);

	//prettyPrinter()
}
