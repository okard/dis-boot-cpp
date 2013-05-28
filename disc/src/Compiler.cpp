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

#include <iostream>

#include <plf/base/SourceFile.hpp>

using namespace plf;
using namespace dis;

Compiler::Compiler()
	: parser_(lexer_)
{
	
}

Compiler::~Compiler()
{
	
}

int Compiler::run(int argc, char *argv[])
{
	//- parse arguments
	
	if(argc >= 2)
		testLex(argv[1]);
	
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
	auto src = std::make_shared<SourceFile>();
	src->open(filename);
	
	Lexer lexer;
	lexer.open(src);
	
	Token tok;
	do
	{
		tok = lexer.next();
		std::cout << "Token: " << dis::toString(tok.id);
		switch(tok.id)
		{
			case TokenId::Ident: 
			case TokenId::StringLiteral:
				std::cout << " Buffer: " << std::string(tok.buffer->ptr(), tok.buffer->size());
				break;
		}
		std::cout << std::endl;
	}
	while(tok.id != TokenId::Eof);
}
