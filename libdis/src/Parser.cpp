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
#include <plf/ast/Type.hpp>

using namespace dis;
using namespace plf;

/**
* Error Result from parsing function
*/
class ErrorDecl : public Declaration
{
public:
	ErrorDecl() 
		: Declaration(NodeKind::Error)
	{}
	
	ErrorDecl(bool eof) 
		: Declaration(NodeKind::Error), eof(eof)
	{}
	
	bool eof = false;
	//EOF
	//Location
	//Message
	//Token?
	//type? Decl/Stmt/Expr/Type
	
	//print function?
};


//Exception with Token


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
			throw Exception("parse(): Not yet implemented");
	}
}

////////////////////////////////////////////////////////////////////////
// Declaration
////////////////////////////////////////////////////////////////////////

/*
* parse all declarations
*/
DeclPtr Parser::parseDeclaration()
{
	//flags (public,private,protected) pub priv prot
	
	DeclFlags flags;
	parseDeclFlags(flags); //parseDeclFlags(DeclFlags&);
	
	//assign flags before
	DeclPtr decl;
	
	switch(tok_.id)
	{	
		case TokenId::KwPackage:  	
			decl = parsePackageDecl(); 
			//todo remove here? nested packages?
			break;
		case TokenId::KwImport:
			decl = parseImportDecl();
			break;
		case TokenId::KwDef: 
			decl = parseFunctionDecl();
			break;
		case TokenId::KwTrait: 
			decl = parseTrait();
			break;
		case TokenId::KwType:
			throw FormatException("parseDeclaration: parsing type not implemented");
			break;
		case TokenId::KwObj:
			decl = parseClass();
			break;
		case TokenId::KwVar: 		
			decl = parseVariable();
			break;
		case TokenId::KwLet:
		  	throw FormatException("parseDeclaration: parsing let not implemented");
			break;
		case TokenId::Eof:
			return Node::create<ErrorDecl>(true);
		default:
			return Node::create<ErrorDecl>();
	}
	
	decl->flags = flags;
	
	return decl;
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
plf::DeclPtr Parser::parsePackageDecl()
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
	// no other package declarations?
	NodePtr decl;
	while((decl = parseDeclaration())->kind() != NodeKind::Error)
	{
		decl->parent = pkg;
		pkg->decls.push_back(decl->to<Declaration>());
	}
	
	//error declarations not allowed
	if( decl
	&&  decl->kind() == NodeKind::Error
	&&  !decl->to<ErrorDecl>()->eof)
	{
		throw FormatException("Error during parsing package");
	}

	return pkg;
}

/**
* import a.b.c;
* import d = a.b.c;
*/
plf::DeclPtr Parser::parseImportDecl()
{
	assert(tok_.id == TokenId::KwImport);
	
	next();
	
	if(tok_.id != TokenId::Ident)
		throw FormatException("Expected <Ident> at import declaration: L %d C %d", tok_.loc.line, tok_.loc.column);
	
	auto import = Node::create<ImportDecl>();
	import->ident = tok_.buffer;
	
	// can has = or .
	
	
	//end with semicolon
	checkNext(TokenId::Semicolon);
	return import;
}

/*
* def name [(params)] [: rettype] body
*/
plf::DeclPtr Parser::parseFunctionDecl()
{
	assert(tok_.id == TokenId::KwDef);
	
	auto func = Node::create<FunctionDecl>();
	
	//function name
	checkNext(TokenId::Ident);
	func->name = tok_.buffer;
	next();
	
	//parameter
	if(tok_.id == TokenId::ROBracket)
	{
		next();
		//parse params
		parseFuncParameter(*func);
		
		check(TokenId::RCBracket);
		next();
	}
	
	//return type
	if(tok_.id == TokenId::Colon)
	{
		//parseDataType
		throw plf::FormatException("parseFunctionDecl: Return type parsing not implemented");
	}
	
	// direct expression body: = expr;
	if(tok_.id == TokenId::Assign)
	{
		throw plf::FormatException("parseFunctionDecl: = <expr>; not implemented");
		
		//parse expression
		next();
		auto expr = parseExpression();
		check(TokenId::Semicolon);
		next();
		
		//setup return
		auto exprStmt = Node::create<ExprStmt>();
		exprStmt->expr = expr;
		exprStmt->expr->parent = exprStmt;
		func->body = exprStmt;
		func->body->parent = func;
		return func;
	}
	
	if(tok_.id == TokenId::COBracket)
	{
		func->body = parseStatement();
		func->body->parent = func;
		return func;
	}
	
	//declaration only
	if(tok_.id == TokenId::Semicolon)
	{
		return func;
	}
	
	
	throw plf::FormatException("parseFunctionDecl: invalid funcdecl");
}

/**
* Function parameter parsing
* {[var|let] ident: datatype }
*/
void Parser::parseFuncParameter(plf::FunctionDecl& func)
{
	if(tok_.id == TokenId::RCBracket)
		return;
	
	while(true)
	{
		bool readonly = false;
		
		//prefix
		if(tok_.id == TokenId::KwVar)
			next();
		else if(tok_.id == TokenId::KwLet)
		{
			readonly = true;
			next();
		}
		
		//ident
		check(TokenId::Ident);
		BufferPtr id = tok_.buffer;
		next();
		
		TypePtr type = UnkownType::getInstance();
		//optional : <datatype>
		if(tok_.id == TokenId::Colon)
		{
			next();
			type = parseDataType();
		}
		
		func.params.push_back(FunctionParameter(readonly, id, type));
		
		//, or break
		if(tok_.id == TokenId::Comma)
			continue;
		else
			return;
	}
}

/*
* var name [: type] [= init];
*/
plf::DeclPtr Parser::parseVariable()
{
	assert(tok_.id == TokenId::KwVar);
	throw plf::FormatException("parseVariable: not yet implemented");
}

/**
* Parse a class
*/
plf::DeclPtr Parser::parseClass()
{
	assert(tok_.id == TokenId::KwObj);
	throw plf::FormatException("parseClass: Not yet implemented");
	
	//tpl
}

/**
* Parse a trait
*/
plf::DeclPtr Parser::parseTrait()
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
			return parseBlockStmt();
			break;
		default:
			break;
	}
	
	//try declaration parsing:
	auto decl = parseDeclaration();
	if(decl && decl->kind() != NodeKind::Error)
	{
		auto declStmt = Node::create<DeclStmt>();
		
		//check kinds for allowed DeclStmt
		
		declStmt->decl = decl;
		return declStmt;
	}
	
	//try expression parsing
	//parseExprStmt
		
		
	//if nothing matched error:
	throw Exception("parseStatement: Not yet fully implemented");
	//if ; parseStatment if no error return block stmt?
}


plf::StmtPtr Parser::parseBlockStmt()
{
	assert(tok_.id == TokenId::COBracket);
	auto blockStmt = Node::create<BlockStmt>();
	
	next();
	while(tok_.id != TokenId::CCBracket)
	{
		auto stmt = parseStatement();
		blockStmt->statements.push_back(stmt);
	}
	
	assert(tok_.id == TokenId::CCBracket);
	next();
	
	return blockStmt;
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
	
	
	//literals
	
	//keyword expressions
	//IfExpr
	//SwitchExpr
	
	//call expressions
	
	//connected expressions:
	
	//operator
	//unary
	//binary
	//tenary


	//return Node::create<ErrorExpr>();
}



////////////////////////////////////////////////////////////////////////
// DataType
////////////////////////////////////////////////////////////////////////

/**
* : id
* : id.<datatype>
* : id[]
* : @<datatype>
* : &<datatype>
* : ~<datatype>
* : [] 
*/
TypePtr Parser::parseDataType()
{
	//simple id
	
	throw FormatException("parseDataType: not yet implemented");
}


////////////////////////////////////////////////////////////////////////
// Utility Functions
////////////////////////////////////////////////////////////////////////

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
		throw plf::FormatException("(%d, %d) Invalid Token expected %s but get %s", tok_.loc.line, tok_.loc.column, toString(id), toString(tok_.id));
	}
}

/// Check next token
void Parser::checkNext(TokenId id)
{
	next();
	check(id);
}
