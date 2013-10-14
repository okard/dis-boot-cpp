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

#include <cassert>

#include <plf/base/Exception.hpp>
#include <plf/base/FormatException.hpp>
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
	ErrorNode() 
		: Node(NodeKind::Error)
	{}
	
	ErrorNode(bool eof) 
		: Node(NodeKind::Error), eof(eof)
	{}
	
	bool eof = false;
	//EOF
	//Location
	//Message
	//Token?
	//type? Decl/Stmt/Expr/Type
	
	//print function?
};



Parser::Parser(Lexer& lex)
	: lexer_(lex)
{
	
}

Parser::~Parser()
{
}

//starting parse function
NodePtr Parser::parse()
{
	next(); //initial token
	
	switch(tok_.id)
	{
		//decl
		case TokenId::KwPackage:
		case TokenId::KwDef:
			return parseDeclaration();
			break;
		//stmt
		
		//if stmtexpr return expr
		//expr
		
		default:
			throw Exception("Not yet implemented");
	}
}

/*
* parse all declarations
*/
NodePtr Parser::parseDeclaration()
{
	//flags (public,private,protected) pub priv prot
	
	DeclFlags flags;
	parseDeclFlags(flags); //parseDeclFlags(DeclFlags&);
	
	//assign flags before
	
	switch(tok_.id)
	{	
		case TokenId::KwPackage:  	return parsePackage(); //todo remove here? nested packages?
		case TokenId::KwImport:  	return parseImport();
		case TokenId::KwDef: 		return parseFunction();
		case TokenId::KwTrait: 		return parseTrait();
		case TokenId::KwType: 		break;
		case TokenId::KwObj: 		return parseClass();
		case TokenId::KwVar: 		return parseVariable();
		case TokenId::KwLet:  		break;
	
		case TokenId::Eof:
			return Node::create<ErrorNode>(true);
		default:
			return Node::create<ErrorNode>();
	}
	
	return Node::create<ErrorNode>();
}

/*
* Parse declarations flags
*/
void Parser::parseDeclFlags(DeclFlags& flags)
{	
	switch(tok_.id)
	{
		case TokenId::KwPub:
			flags = flags | DeclFlags::Public;
			next();
			parseDeclFlags(flags);
			break;
		case TokenId::KwPriv:
			flags = flags | DeclFlags::Private;
			next();
			parseDeclFlags(flags);
			break;
		case TokenId::KwProt:
			flags = flags | DeclFlags::Protected;
			next();
			parseDeclFlags(flags);
			break;
		//static
		//final
		//abstract
		//const
		default:
			break;
	}
}

/*
* package a.b.c;
* package ident .ident .ident ;
*/
plf::NodePtr Parser::parsePackage()
{
	assert(tok_.id == TokenId::KwPackage);
	
	auto pkg = Node::create<PackageDecl>();
	
	//parse path
	checkNext(TokenId::Ident);
	while(tok_.id == TokenId::Ident)
	{
		pkg->path.push_back(tok_.buffer);
		next();
		
		if(tok_.id == TokenId::Dot)
			next();
	}
	
	//ends with semicolon
	check(TokenId::Semicolon);
	next();
	
	//read all declaration in package
	NodePtr decl;
	while((decl = parseDeclaration())->kind() != NodeKind::Error)
	{
		decl->parent = pkg;
		pkg->decls.push_back(decl->to<Declaration>());
	}
	
	//error declarations not allowed
	if( decl
	&&  decl->kind() == NodeKind::Error
	&&  !decl->to<ErrorNode>()->eof)
	{
		throw FormatException("Error during parsing package");
	}

	return pkg;
}

/**
* import a.b.c;
* import d = a.b.c;
*/
plf::NodePtr Parser::parseImport()
{
	assert(tok_.id == TokenId::KwImport);
	throw FormatException("Not implemented");
	
}

/*
* def name [(params)] [: rettype] body
*/
plf::NodePtr Parser::parseFunction()
{
	assert(tok_.id == TokenId::KwDef);
	
	auto func = Node::create<FunctionDecl>();
	
	//function name
	checkNext(TokenId::Ident);
	func->ident = tok_.buffer;
	
	if(peek(1, TokenId::ROBracket))
	{
		next();
		//parse params
		
		//skip RCBracket
	}
	
	
	//if ':' parse type
	
	//if = parse expr
	//if { parse block
	//if ; end
	
	
	
	throw plf::FormatException("Not implemented");
}

/*
* var name [: type] [= init];
*/
plf::NodePtr Parser::parseVariable()
{
	assert(tok_.id == TokenId::KwVar);
	throw plf::FormatException("Not implemented");
	
}

/**
* Parse a class
*/
plf::NodePtr Parser::parseClass()
{
	assert(tok_.id == TokenId::KwObj);
	throw plf::FormatException("Not implemented");
	
	//tpl
}

/**
* Parse a trait
*/
plf::NodePtr Parser::parseTrait()
{
	assert(tok_.id == TokenId::KwTrait);
	throw plf::FormatException("Not implemented");
	
	//tpl
}

////////////////////////////////////////////////////////////////////////
// Statements
////////////////////////////////////////////////////////////////////////

/**
* Parse statements
*/
StmtPtr Parser::parseStatement()
{
	switch(tok_.id)
	{
		case TokenId::KwIf:
			throw plf::Exception("parsing 'IfStmt' not implemented");
			break;
		case TokenId::KwFor:
			throw plf::Exception("parsing 'ForStmt' not implemented");
			break;
		case TokenId::KwWhile:
			throw plf::Exception("parsing 'WhileStmt' not implemented");
			break;
		case TokenId::COBracket:
			throw plf::Exception("parsing 'BlockStmt' not implemented");
			break;
		
		default:
			throw Exception("Not yet implemented");
	}
	
	//decl statments
	//parseDeclaration() if(error and not eof)
	//parseExpression
	//expr statements
	
	
	//if ; parseStatment if no error return block stmt?
}


////////////////////////////////////////////////////////////////////////
// Expression
////////////////////////////////////////////////////////////////////////

/**
* Parse Expressions
*/
ExprPtr Parser::parseExpression()
{
	throw Exception("Expression parsing not yet implemented");
	
	//unary
	//binary
	
	//IfExpr
	//SwitchExpr
	
	//return Node::create<ErrorExpr>();
}

////////////////////////////////////////////////////////////////////////
// DataType
////////////////////////////////////////////////////////////////////////

/**
* : id
* : id.datatype
* : id[]
* : @datatype
* : &datatype
* : ~datatype
* : [] 
*/
plf::NodePtr Parser::parseDataType()
{
	
	
	return Node::create<ErrorNode>();
}

/// Read next token
void Parser::next()
{
	tok_ = lexer_.next();
}

/// do a peek check with a TokenId
bool Parser::peek(int count, TokenId id)
{
	return lexer_.peek(count).id == id;
}

/// Check current token
void Parser::check(TokenId id)
{
	if(tok_.id != id)
	{
		//TODO SyntaxError
		throw plf::FormatException("Invalid Token expected %s but get %s", toString(id), toString(tok_.id));
	}
}

/// Check next token
void Parser::checkNext(TokenId id)
{
	next();
	check(id);
}
