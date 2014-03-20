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
#include <iostream>

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
	tok_.id = TokenId::NotInitialized;
}

Parser::~Parser()
{
}

//starting parse function
NodePtr Parser::parse()
{
	//first call initialize token
	if(tok_.id == TokenId::NotInitialized)
	{
		next();
	}

	//std::cout << "DEBUG: parse() TOK: " << toString(tok_.id) << std::endl;

	if(tok_.id == TokenId::Eof)
	{
		return Node::create<EofNode>();
	}

	NodePtr n;

	//try_parse Decl
	n = parseDeclaration();
	if(n && n->kind != NodeKind::Error)
		return n;

	//try_parse Stmt
	n = parseStatement();
	if(n && n->kind != NodeKind::Error)
		return n;

	//try_parse Expr
	//will returned as ExprStmt

	//throw FormatException("parse(): [%s] Not yet implemented", toString(tok_.id));

	throw FormatException("parse(): No valid stuff to parse [Token: %s]", toString(tok_.id));
}

////////////////////////////////////////////////////////////////////////
// Declaration
////////////////////////////////////////////////////////////////////////

/*
* parse all declarations
*/
DeclPtr Parser::parseDeclaration()
{
	//TODO parse options to exclude mod

	//flags (public,private,protected) pub priv prot
	DeclFlags flags = DeclFlags::None;
	parseDeclFlags(flags); //parseDeclFlags(DeclFlags&);
	
	//assign flags before
	DeclPtr decl;
	
	switch(tok_.id)
	{	
		case TokenId::KwMod:
			decl = parseModDecl();
			break;
		case TokenId::KwUse:
			decl = parseUseDecl();
			break;
		case TokenId::KwDef: 
			decl = parseFunctionDecl();
			break;
		case TokenId::KwTrait: 
			decl = parseTraitDecl();
			break;
		case TokenId::KwStruct:
			decl = parseStructDecl();
			break;
		case TokenId::KwEnum:
			decl = parseEnumDecl();
			break;
		case TokenId::KwType:
			decl = parseTypeDecl();
			break;
		//Instance Declarations
		case TokenId::KwVar: 
		case TokenId::KwLet:
		case TokenId::KwConst:		
			decl = parseInstanceDecl();
			break;

		//TODO think over error node stuff
		case TokenId::Eof:
			return Node::create<ErrorDecl>(true);
		default:
		{
			auto err = Node::create<ErrorDecl>();
			err->line = tok_.loc.line;
			err->col = tok_.loc.column;
			err->id = tok_.id;
			//signal callback
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

/**
 * @brief Parse a module declaration
 *	      mod ident;
 *		  mod ident {}
 * @return
 */
DeclPtr Parser::parseModDecl()
{
	throw Exception("Not yet implemented");
}

/**
 * @brief parse a use Declaration
 *		  use abc::abc
 *		  use ident = abc::abc;
 *		  use this = abc::abc
 *		  ?use("file.dis")?
 * @return a use decl or error decl?
 */
DeclPtr Parser::parseUseDecl()
{
	assert(tok_.id == TokenId::KwUse);
	next();

	if(tok_.id != TokenId::Ident)
		throw FormatException("Expected <Ident> at use declaration: L %d C %d", tok_.loc.line, tok_.loc.column);

	auto import = Node::create<UseDecl>();
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
	tok_.buffer = std::make_shared<Buffer>();
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
	
	
	throw plf::FormatException("parseFunctionDecl: invalid function declaration");
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
		tok_.buffer = std::make_shared<Buffer>();
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
*
* trait ident(param) : inherit type { }
*/
plf::DeclPtr Parser::parseTraitDecl()
{
	assert(tok_.id == TokenId::KwTrait);


	throw plf::FormatException("Parsing traits not yet implemented");
}

/**
 * @brief Parse a struct declaration
 *        struct <name> { <flag name : type>* }
 * @return
 */
DeclPtr Parser::parseStructDecl()
{
	assert(tok_.id == TokenId::KwStruct);

	throw plf::FormatException("Parsing structs not yet implemented");
}

DeclPtr Parser::parseEnumDecl()
{
	assert(tok_.id == TokenId::KwEnum);

	throw plf::FormatException("Parsing enums not yet implemented");
}

DeclPtr Parser::parseTypeDecl()
{
	assert(tok_.id == TokenId::KwType);

	throw plf::FormatException("Parsing type aliases not yet implemented");
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
	tok_.buffer = std::make_shared<Buffer>();
	
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
	if(decl && decl->kind != NodeKind::Error)
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

/*
 Expression Parsing 'Operator' Pre

 Unary Prefix
	!<expr>
	&<expr>
	-<expr>
	++<expr>
	--<expr>

 Unary Postfix
	<expr>()
	<expr>++
	<expr>--



	atoms
		(<expr>)
		Ident
		Literal
		
		

*/

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
			tok_.buffer = std::make_shared<Buffer>();
			expr = l;
			next();
			break;
		}
			
		case TokenId::FloatLiteral:
		{
			auto l = Node::create<FloatLiteral>();
			l->rawValue = tok_.buffer;
			tok_.buffer = std::make_shared<Buffer>();
			expr = l;
			next();
			break;
		}
			
		case TokenId::HexLiteral:
		{
			auto l = Node::create<HexLiteral>();
			l->rawValue = tok_.buffer;
			tok_.buffer = std::make_shared<Buffer>();
			expr = l;
			next();
			break;
		}
			
		case TokenId::BinaryLiteral:
		{
			auto l = Node::create<BinaryLiteral>();
			l->rawValue = tok_.buffer;
			tok_.buffer = std::make_shared<Buffer>();
			expr = l;
			next();
			break;
		}
			
		case TokenId::StringLiteral:
		{
			auto l = Node::create<StringLiteral>();
			l->rawValue = tok_.buffer;
			tok_.buffer = std::make_shared<Buffer>();
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

ExprPtr Parser::parseExprAtom()
{
	//special '('

	//prefix
	//- UnaryOpPrefix

	//main
	//-ident
	//-literal

	//postfix

	//- UnaryOpPostfix
	//- ArrayAccess
	//- Call

	throw Exception("Not yet implemented");
}

ExprPtr Parser::parseExprBinary(int min_prec)
{
	throw Exception("Not yet implemented");
}

/*
* if(<expr>){}
* else if(<expr>){}
* else{}
*/
ExprPtr Parser::parseIfExpr()
{
	throw Exception("If-Expression parsing not yet implemented");
}

/*
* match(<expr>)
*/
ExprPtr Parser::parseMatchExpr()
{
	throw Exception("Match-Expression parsing not yet implemented");
}

//return operator precedence
int Parser::op_prec(TokenId id)
{
	switch(id)
	{

	//Binary:
	case TokenId::Plus:
	case TokenId::Minus:
		return 0;

	case TokenId::Mul:
	case TokenId::Div:
		//pow
		return 1;
		
	//Unary:

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
* (precedence climbing?)
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
			tok_.buffer = std::make_shared<Buffer>();
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

		case TokenId::ConstraintStart:
			throw FormatException("parseDataType: parsing of type constraints not yet implemented");

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
	lexer_.next(tok_);
}

/// do a peek check with a TokenId
bool Parser::peek(int count, TokenId id)
{
	throw FormatException("Parser::peek: not yet implemented");
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
