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


#include <plf/base/Exception.hpp>
#include <plf/ast/Declaration.hpp>

#include <plf/sem/TypeResolver.hpp>
#include <plf/sem/Interpreter.hpp>

#include <plf/utils/DotPrinter.hpp>

#include <dis/Printer.hpp>

#include "ColoredOutput.hpp"

using namespace plf;
using namespace dis;


/*
 { "-parse", 1, std::function<void(argc, argv)> } 
 rest


Command Line Options:

	-compilation_unit <jsonfile> // use a file for all configuration


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

	//when -lex, -parse, etc is used remove default flags

	//- parse arguments
	for(int i=1; i < argc; i++)
	{
		//add flags
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
			//multiple CompilationUnits in this case


		//TODO all files using only one central crate?


		//not an option so it is a source file

		//TODO flag steps (lex,parse,semantic,compile,link)
		auto sourcePtr = SourceManager::getInstance().loadFile(argv[i]);
		units_.push_back(CompilationUnit(sourcePtr));
		//std::cout << "File: " << argv[i] << " id: "<< sourcePtr->getId() << " src: " << sourcePtr->identifier()  << std::endl;
	}


	bool result = false;

	//1. first parse ( Lex->Parse->AST )
	std::cout << "->parse" << std::endl;
	result = parse();

	if(!result)
		return -2;

	//2. semantic stuff (AST->DAST)
	std::cout << "->semantic" << std::endl;
	result = semantic();

	//5. codegen ( Ast -> ObjectFile)
	std::cout << "->compile" << std::endl;
	result = compile();

	//6. link ( ObjectFile -> Binary)
	std::cout << "->link" << std::endl;
	result = link();
	
	return 0;
}

bool Compiler::parse()
{
	bool parse_valid = true;

	//std::cout << "CompilationUnits: " << units_.size() << std::endl;
	for(unsigned int i = 0; i < units_.size(); i++)
	{
		CompilationUnit& unit =  units_[i];

		if(!unit.sourcePtr)
		{
			parse_valid = false;
			std::cerr << "ICE: invalid source in CompilationUnit" << std::endl;
			continue;
		}

		std::cout << "->parse[" << unit.sourcePtr->identifier() << "]" << std::endl;


		try
		{
			//lexer
			lexer_.open(unit.sourcePtr);
			parser_.reset();

			//parse it complete
			NodePtr n;
			do
			{
				n = parser_.parse();


				if(n->kind != NodeKind::Error)
				{
					auto decl = n->to<Declaration>(); //Declaration.hpp required because of knowing inheritance
					//add to crate
					unit.crate.decls.push_back(decl);
				}
			}
			while(n && n->kind != NodeKind::Error);
		}
		catch(std::exception& exc)
		{
			parse_valid = false;
			std::cerr << red << "Parser Exception: " << reset_color << exc.what() << std::endl;
		}
	}

	return parse_valid;
}

bool Compiler::semantic()
{
	//instances the semantic visitor stuff

	plf::TypeResolver typeResolver;
	//plf::Interpreter interpreter;

	for(CompilationUnit& unit: units_)
	{
		//semantic steps

		//-> import solving for mods, use decls
		//-> resolve imports (Ast)
			//resolve imports (&crate)
				// root crate "exports" module structure
				// resolve the modules defined by root crate and add sources/asts for modules

		//-> StructureTest? -> require main, enum fields, ....

		//-> type solving?
		typeResolver.run(unit.crate);

		//-> Run Interpreter for compile time execution

		//-> Optimizer stuff (constant folding?)

		//-> Create symbol table?
	}

	return true;
}

bool Compiler::compile()
{
	//Instanciate the right codegen backend

	return true;
}

bool Compiler::link()
{
	//Instanciate the right link backend

	return true;
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
			std::cout.write(tok.buffer->str(), tok.buffer->size());
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
	PrettyPrinter p;
	ParamPtr pp;
	NodePtr n;
	do
	{
		n = parser.parse();

		//print:
		if(n->kind != NodeKind::Error)
			p.dispatch(n);
	}
	while(n && n->kind != NodeKind::Error);

	//prettyPrinter()
}
