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


#include <plf/base/SourceFile.hpp>
#include <plf/base/SourceManager.hpp>

using namespace plf;
using namespace dis;

/*
 { "-parse", 1, std::function<void(argc, argv)> } 
 rest
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
		
	}
		
	
	//auto src = std::make_shared<SourceFile>();
	//src->open(argv[1]);
	//lexer_.open(src);
	
	
	//1. open files
	//2. parse files (Lex->Parse->Ast)
	//3. resolve imports (Ast)
	//4. semantic run (Ast -> DAst)
	//5. codegen ( Ast -> ObjectFile)
	//6. link ( ObjectFile -> Binary)
	
	return 0;
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
		tok = lexer.next();
		std::cout << "Token(" << tok.loc.line << ", " << tok.loc.column << "): " << dis::toString(tok.id);
		switch(tok.id)
		{
			case TokenId::Ident: 
			case TokenId::StringLiteral:
				std::cout << " Buffer: ";
				std::cout.write(tok.buffer->ptr(), tok.buffer->size());
				break;
				
			default:
				break;
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
	
	auto n = parser.parse();
	
	//prettyPrinter()
}
