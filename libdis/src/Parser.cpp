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
#include <dis/Parser.hpp>

#include <plf/ast/Declaration.hpp>
#include <plf/ast/Statement.hpp>
#include <plf/ast/Expression.hpp>

using namespace dis;
using namespace plf;

/**
* Error Result from parsing function
*/
class ErrorNode : public Node
{
public:
	ErrorNode() : Node(NodeKind::Error)
	{}
	
	//Location
	//Message
	//Token?
};



Parser::Parser(Lexer& lex)
	: lexer_(lex)
{
	
}

Parser::~Parser()
{
}

NodePtr Parser::parse()
{
	next();
	
	switch(tok_.id)
	{
		case TokenId::KwPackage:
		case TokenId::KwDef:
			return parseDeclaration();
			break;
	}
}

NodePtr Parser::parseDeclaration()
{
	//flags (public,private,protected) pub priv prot
	
	switch(tok_.id)
	{	
		case TokenId::KwPackage: 
			break;
		case TokenId::KwDef: 
			break;
		case TokenId::KwTrait: 
			break;
		case TokenId::KwType: 
			break;
		case TokenId::KwObj: 
			break;
		case TokenId::KwVar: 
			break;
		case TokenId::KwLet: 
			break;
	
		default:
			return std::make_shared<ErrorNode>();
	}
	
}

plf::NodePtr Parser::parsePackage()
{
	//assert(tok.id == TokenId::KwPackage);
	
	//auto pkg = std::make_shared<PackageDecl>();
	auto pkg = Node::create<PackageDecl>();
	
	//PackageDecl
	
	NodePtr decl;
	while((decl = parseDeclaration())->kind() != NodeKind::Error)
	{
		decl->parent = pkg;
		pkg->Decls.push_back(decl->to<Declaration>());
	}
	
	return pkg;
}

plf::NodePtr Parser::parseFunction()
{
	
}

plf::NodePtr Parser::parseVariable()
{
	
}


StmtPtr Parser::parseStatement()
{
	
}

ExprPtr Parser::parseExpression()
{
	
}

void Parser::next()
{
	tok_ = lexer_.next();
}
