/*
Dis Programming Language Frontend Library

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
#pragma once
#ifndef __DIS_PARSER_HPP__
#define __DIS_PARSER_HPP__

#include <plf/ast/Node.hpp>

#include "Lexer.hpp"

namespace plf {
	enum class DeclFlags : unsigned char;
	class FunctionDecl;
}

namespace dis {

/**
* Dis Lang Parser
*/
class Parser
{
private:
	Lexer& lexer_;
	
	Token tok_;
	//TODO Tokenstack?
	//DeclFlags
	
	//error handler?
public:

	Parser(Lexer& lex);
	~Parser();


	plf::NodePtr parse();


private:
	////////////////////////////////////////////////////////////////////
	// Declaration
	
	plf::DeclPtr parseDeclaration();
	
	void parseDeclFlags(plf::DeclFlags& flags);
	
	plf::DeclPtr parsePackageDecl();
	plf::DeclPtr parseImportDecl();
	plf::DeclPtr parseFunctionDecl();
	void parseFuncParameter(plf::FunctionDecl& func);
	
	plf::DeclPtr parseClass();
	plf::DeclPtr parseTrait();
	//enum, struct, union
	plf::DeclPtr parseTypeDecl();
	
	
	plf::DeclPtr parseInstanceDecl();
	
	//parse a Ident : Type
	
	////////////////////////////////////////////////////////////////////
	// Statements
	plf::StmtPtr parseStatement();
	
	plf::StmtPtr parseBlockStmt();
	
	//parseForStmt
	//parseWhileStmt
	//parseDoWhileStmt
	//parseIfStmt
	//parseSwitchStmt
	
	
	////////////////////////////////////////////////////////////////////
	// Expressions
	plf::ExprPtr parseExpression();
	
	//parse
	
	//parseIfExpr()
	//parseSwitchExpr()
	
	////////////////////////////////////////////////////////////////////
	// DataType
	plf::TypePtr parseDataType();
	
	////////////////////////////////////////////////////////////////////
	// Utils
	void next();
	
	bool peek(int count, TokenId id);
	//sync();
	
	void check(TokenId id);
	void checkNext(TokenId id);
};


} //end namespace dis

#endif // __DIS_PARSER_HPP__
