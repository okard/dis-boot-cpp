/*
Dis Programming Language Frontend Library

Copyright (c) 2014 okard

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
#ifndef DIS_PARSER_HPP
#define DIS_PARSER_HPP

#include <plf/ast/Node.hpp>

#include "Lexer.hpp"

namespace plf {
	//Declaration
	enum class DeclFlags : unsigned char;
	class FunctionDecl;
	//Expression
	enum class UnaryOperator;
	enum class BinaryOperator;
}

namespace dis {

/**
* Dis Lang Parser
*/
class Parser
{
private:
	Lexer& lexer_;
	
	//TODO token peek/lookahead
	//Stack<Token>/std::dequeue<Token>
	Token tok_;

	//DeclFlags
	//error handler?

public:

	Parser(Lexer& lex);
	~Parser();

	//parse into crate
	plf::NodePtr parse();

	void reset();

	//TODO template parse function?

private:
	////////////////////////////////////////////////////////////////////
	// Declaration
	
	plf::DeclPtr parseDeclaration();
	
	void parseDeclFlags(plf::DeclFlags& flags);

	plf::DeclPtr parseModDecl();
	plf::DeclPtr parseUseDecl();

	plf::DeclPtr parseFunctionDecl(bool skipIdent);
	void parseFuncParameter(plf::FunctionDecl& func);
	
	plf::DeclPtr parseTraitDecl();

	plf::DeclPtr parseStructDecl();

	plf::DeclPtr parseEnumDecl();

	//alias etc
	plf::DeclPtr parseTypeDecl();
	
	//const, let, var
	plf::DeclPtr parseInstanceDecl();
	
	//parse a Ident : Type

	//parseAttribute
	
	////////////////////////////////////////////////////////////////////
	// Statements
	plf::StmtPtr parseStatement();
	
	plf::StmtPtr parseBlockStmt();

	plf::StmtPtr parseForStmt();
	plf::StmtPtr parseWhileStmt();

	//parseDoWhileStmt?

	plf::StmtPtr parseIfStmt();
	plf::StmtPtr parseMatchStmt();
	
	
	////////////////////////////////////////////////////////////////////
	// Expressions
	plf::ExprPtr parseExpression();

	plf::ExprPtr parseExprAtom();
	plf::ExprPtr parseExprBinary(int min_prec);
	
	plf::ExprPtr parseIfExpr();
	plf::ExprPtr parseMatchExpr();

	////////////////////////////////////////////////////////////////////
	// DataType
	plf::TypePtr parseDataType();
	
	////////////////////////////////////////////////////////////////////
	// Utils
	void next();
	
	bool peek(int count, TokenId id);
	//void sync(TokenId id); //for better error diagnostic?
	
	void check(TokenId id);
	void checkNext(TokenId id);

	//create error nodes?

	//TODO create error, warning signals and callbacks

	void warning(const char* msg, ...);
	void error(const char* msg, ...);
};


} //end namespace dis

#endif // DIS_PARSER_HPP
