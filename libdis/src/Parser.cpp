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

#include <dis/ParserError.hpp>

using namespace dis;
using namespace plf;


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


	//try_parse Decl
	//try_parse Stmt
	//try_parse Expr
	
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
			throw FormatException("parse(): [%s] Not yet implemented", toString(tok_.id));
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
	
	DeclFlags flags = DeclFlags::None;
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
		//Instance Declarations
		case TokenId::KwVar: 
		case TokenId::KwLet:
		case TokenId::KwConst:		
			decl = parseInstanceDecl();
			break;
		case TokenId::Eof:
			return Node::create<ErrorDecl>(true);
		default:
		{
			auto err = Node::create<ErrorDecl>();
			err->line = tok_.loc.line;
			err->col = tok_.loc.column;
			err->id = tok_.id;
			return err;
		}
	}
	
	decl->flags = flags;
	
	return decl;
}

/*
* Parse declarations flags
*/
void Parser::parseDeclFlags(DeclFlags& flags)
{	
	//TODO no multiple pub/priv/prot flags
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
		auto err = decl->to<ErrorDecl>();
		throw FormatException("(%d, %d) Error during parsing package", err->line, err->col);
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
		next();
		func->returnType = parseDataType(); 
	}
	
	// direct expression body: = expr;
	if(tok_.id == TokenId::Assign)
	{
		//parse expression
		next();
		auto expr = parseExpression();
		check(TokenId::Semicolon);
		next();
		
		//setup returnq
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
		next(); //skip it
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
		{
			next();
			continue;
		}
		else
			return;
	}
}

/**
* Parse a class
* class <ident> [ ( <tpl args> ) ] [ : <type> ] { <decls> }
*/
/*plf::DeclPtr Parser::parseClass()
{
	assert(tok_.id == TokenId::KwObj);
	next();
	
	check(TokenId::Ident);
	auto classDecl = Node::create<ClassDecl>();
	classDecl->name = tok_.buffer;
	next();
	
	//template class
	if(tok_.id == TokenId::ROBracket)
	{
		next();
		//parse template arguments ( name: type, ...)
		throw plf::FormatException("parseClass: tpl args parsing not implemented");
		check(TokenId::RCBracket);
		next();
	}
	
	//inheritance
	if(tok_.id == TokenId::Colon)
	{
		next();
		//parse base class
		throw plf::FormatException("parseClass: inheritance parsing not implemented");
	}
	
	check(TokenId::COBracket);
	next();
	
	DeclPtr decl;
	while((decl = parseDeclaration())->kind() != NodeKind::Error)
	{
		decl->parent = classDecl;
		classDecl->decls.push_back(decl);
	}
	
	check(TokenId::CCBracket);
	next();
	
	return classDecl;
}*/

/**
* Parse a trait
*/
plf::DeclPtr Parser::parseTrait()
{
	assert(tok_.id == TokenId::KwTrait);
	throw plf::FormatException("Not implemented");
	
	//tpl
}

/*
* var/let/const name [: type] [= init];
*/
plf::DeclPtr Parser::parseInstanceDecl()
{
	auto inst = Node::create<InstanceDecl>();
	
	//type of instance
	switch(tok_.id)
	{
		case TokenId::KwVar:
			inst->itype = InstanceType::Variable;
			break;
		case TokenId::KwLet:
			inst->itype = InstanceType::Value;
			break;
		case TokenId::KwConst:
			inst->itype = InstanceType::Constant;
			break;
		default:
			throw FormatException("parseInstanceDecl: Invalid Token");
	}
	
	//name
	checkNext(TokenId::Ident);
	inst->name = tok_.buffer;
	
	next();
	
	//datatype
	if(tok_.id == TokenId::Colon)
	{
		next();
		inst->type = parseDataType();
	}
	
	//initializer expression
	if(tok_.id == TokenId::Assign)
	{
		next();
		inst->init = parseExpression();
	}
	
	check(TokenId::Semicolon);
	next();
	
	return inst;
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

/**
* Parses a Block Statement 
*  {}
*/
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
	//TODO change to bottom up parsing via token stack?
	
	ExprPtr expr;
	
	switch(tok_.id)
	{
		//---------------------------
		//literals
		case TokenId::IntLiteral:
		{
			auto l = Node::create<IntegerLiteral>();
			l->rawValue = tok_.buffer;
			expr = l;
			next();
			break;
		}
			
		case TokenId::FloatLiteral:
		{
			auto l = Node::create<FloatLiteral>();
			l->rawValue = tok_.buffer;
			expr = l;
			next();
			break;
		}
			
		case TokenId::HexLiteral:
		{
			auto l = Node::create<HexLiteral>();
			l->rawValue = tok_.buffer;
			expr = l;
			next();
			break;
		}
			
		case TokenId::BinaryLiteral:
		{
			auto l = Node::create<BinaryLiteral>();
			l->rawValue = tok_.buffer;
			expr = l;
			next();
			break;
		}
			
		case TokenId::StringLiteral:
		{
			auto l = Node::create<StringLiteral>();
			l->rawValue = tok_.buffer;
			expr = l;
			next();
			break;
		}
			
		//---------------------------
		//keyword expressions
		case TokenId::KwIf:
			return parseIfExpr();

		case TokenId::KwMatch:
			return parseMatchExpr();

		
		//unary expression prefixed
		
		case TokenId::PlusPlus:		// ++<expr>
		case TokenId::MinusMinus: 	// --<expr>
		case TokenId::And:			// &<expr>
		case TokenId::Tilde:		// ~<expr>
			throw Exception("Unary-Prefix-Expression parsing not yet implemented");
			break;
		
		//sub expression (<expr>)
		case TokenId::ROBracket:
			next();
			expr = parseExpression();
			check(TokenId::RCBracket);
			next();
			break;
		
		default:
			throw Exception("Invalid Expression or not implemented");
			break;
	}
	
	//special: lambda expressions
	
	//call expressions: <expr>(<expr>, ...)
	
	//connected expressions:
	
	switch(tok_.id)
	{
		case TokenId::Semicolon:
			return expr;
			
		//operator:
		//unary postfix
		case TokenId::PlusPlus:
		case TokenId::MinusMinus:
			throw Exception("Unary-Postfix-Expression parsing not yet implemented");
			
		//binary 
		case TokenId::Plus:
		case TokenId::Minus:
		case TokenId::Mul:
		case TokenId::Div:
			throw Exception("Binary-Expression parsing not yet implemented");
			/*
			*/
			
		//tenary
		
		//<expr> ? <expr> : <expr>
		
		//cast: <expr> as <datatype>
		case TokenId::KwAs:
			throw Exception("Cast-Expression parsing not yet implemented");
			
		default:
			throw FormatException("[%s] Invalid Expression or not implemented", toString(tok_.id));
	}

	//return Node::create<ErrorExpr>();
	
	return expr;
}

ExprPtr Parser::parseIfExpr()
{
	throw Exception("If-Expression parsing not yet implemented");
}

ExprPtr Parser::parseMatchExpr()
{
	throw Exception("Match-Expression parsing not yet implemented");
}

//return operator precedence
int Parser::op_prec(TokenId id)
{
	switch(id)
	{
	case TokenId::Plus:
	case TokenId::Minus:
		return 0;

	case TokenId::Mul:
	case TokenId::Div:
		return 1;

	default:
			throw Exception("Not an operator");
	}
}

////////////////////////////////////////////////////////////////////////
// DataType
////////////////////////////////////////////////////////////////////////

/**
* : id
* : id.<datatype>
* : []id
* : @<datatype>
* : &<datatype>
* : ~<datatype>
* : *<datatype>
* : || 
* : <datatype>!<datatype>
* : <datatype>!(<datatype>*)
* -----------------------------
* : ~<datatype>!<datatype>
*/
TypePtr Parser::parseDataType()
{
	TypePtr t = UnkownType::getInstance();
	
	//start tokens
	switch(tok_.id)
	{
		case TokenId::Ident:
		{
			auto type = Node::create<UnsolvedType>();
			type->idents.push_back(tok_.buffer);
			next();
			t = type;
			break;
		}
		
		//heap owned ptr type
		case TokenId::Tilde:
		{
			next();
			auto opt = Node::create<OwnedPtrType>();
			opt->targetType = parseDataType();
			return opt;
		}
			
		//borrowed pointer
		case TokenId::And:
		{
			next();
			auto bpt = Node::create<BorrowedPtrType>();
			bpt->targetType = parseDataType();
			return bpt;
		}
			
		//raw unsafe pointer
		case TokenId::Mul:
		{
			next();
			auto rpt = Node::create<RawPtrType>();
			rpt->targetType = parseDataType();
			return rpt;
		}
		
		case TokenId::SOBracket:
			throw FormatException("parseDataType: parsing of array and map types not implemented");
			break;
			
		default:
			throw FormatException("parseDataType: not yet implemented or invalid type");
	}
	
	//follow up
	switch(tok_.id)
	{
		case TokenId::Dot:	//.
		case TokenId::EPoint: //!
			throw FormatException("parseDataType: not yet implemented");
			break;
		default:
			break;
	}
	
	return t;
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
