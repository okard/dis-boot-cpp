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
#include <cstdarg>
#include <cstdio>
#include <iostream>

#include <plf/base/Exception.hpp>
#include <plf/base/FormatException.hpp>

#include <plf/ast/Declaration.hpp>
#include <plf/ast/Statement.hpp>
#include <plf/ast/Expression.hpp>
#include <plf/ast/Type.hpp>

#include <dis/ParserError.hpp>

#include "Parser_Helper.hpp"

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

	if(tok_.id == TokenId::Unkown)
	{
		throw Exception("something is going wrong, lexer delivers unkown tokenid");
	}

	//end of file no more token
	if(tok_.id == TokenId::Eof)
	{
		return Node::create<EofNode>();
	}

	NodePtr n;

	//try_parse Decl
	n = parseDeclaration();
	if(n && n->kind != NodeKind::Error)
		return n;

	//test for eof?

	//try_parse Stmt (returns also an expression statement)
	n = parseStatement();
	if(n && n->kind != NodeKind::Error)
		return n;

	//try_parse Expr -> will returned as ExprStmt
	if(n && n->kind == NodeKind::ExprStmt)
		return n->to<ExprStmt>()->expr;

	throw FormatException("parse(): No valid stuff to parse [Token: %s]", toString(tok_.id));
}

void Parser::reset()
{
	tok_ = Token();
	tok_.id = TokenId::NotInitialized;
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

	//parse unsafe?
	if(tok_.id == TokenId::KwUnsafe)
		throw Exception("unsafe not yet implemented");
	
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
			//unsafe only here?
			decl = parseFunctionDecl(false);
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
			decl = parseInstanceDecl(ParseOption::CheckSemicolon);
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

		case TokenId::KwUnsafe:
			flags = flags | DeclFlags::Unsafe;
			next();
			parseDeclFlags(flags);
			break;

		//TODO how to define following stuff:
		//-static
		//-final
		//-abstract
		//-const

		//TODO unsafe also here?
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
	assert(tok_.id == TokenId::KwMod);
	next();

	auto modDecl = Node::create<ModDecl>();

	check(TokenId::Ident);
	modDecl->name = transfer(tok_.buffer);
	next();

	//only mod decl
	if(tok_.id == TokenId::Semicolon)
	{
		next(); //skip semicolon
		return modDecl;
	}

	//module with content
	if(tok_.id == TokenId::COBracket)
	{
		next();
		while(tok_.id != TokenId::CCBracket)
		{
			auto decl = parseDeclaration();

			//only add non error nodes
			if(decl->kind != NodeKind::Error)
				modDecl->decls.push_back(decl);

			//check for error type?
		}
		assert(tok_.id == TokenId::CCBracket);
		next(); //skip '}'
		return modDecl;
	}

	throw FormatException("Invalid token in mod declaration: %s L %d C %d", toString(tok_.id), tok_.loc.line, tok_.loc.column);
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
plf::DeclPtr Parser::parseFunctionDecl(bool skipIdent)
{
	assert(tok_.id == TokenId::KwDef);
	
	auto func = Node::create<FunctionDecl>();
	
	//function name
	if(skipIdent)
	{
		next(); //skip kw def
	}
	else
	{
		next();

		//pure compile time function
		if(tok_.id == TokenId::Dollar)
		{
			next();
			func->ctfFunc = true;
		}

		check(TokenId::Ident);
		func->name = transfer(tok_.buffer);
		next();
	}
	
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

		//Optional ; at end (for lambda reusage)
		//check(TokenId::Semicolon);
		if(tok_.id == TokenId::Semicolon)
		{
			next();
		}
		
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
	next();	//skip kw trait

	auto traitdecl = Node::create<TraitDecl>();

	//name
	check(TokenId::Ident);
	traitdecl->name = transfer(tok_.buffer);
	next();

	//tpl parameter
	if(current(TokenId::ROBracket))
	{
		next();
		throw plf::FormatException("Parsing template parameter of traits not yet implemented");
	}

	if(current(TokenId::Colon))
	{
		throw plf::FormatException("Parsing inheritance of traits not yet implemented");
	}

	check(TokenId::COBracket);
	next();
	//parse decl list
	while(tok_.id != TokenId::CCBracket)
	{
		auto decl = parseDeclaration();

		//only add non error nodes
		if(decl->kind != NodeKind::Error)
			traitdecl->decls.push_back(decl);

		//check for error type?
	}
	assert(tok_.id == TokenId::CCBracket);
	next(); //skip '}'

	return traitdecl;
}

/**
 * @brief Parse a struct declaration
 *        struct <name> { <flag name : type>* }
 * @return
 */
DeclPtr Parser::parseStructDecl()
{
	assert(tok_.id == TokenId::KwStruct);

	checkNext(TokenId::Ident);
	auto structDecl = Node::create<StructDecl>();
	structDecl->name = transfer(tok_.buffer);
	next();

	//parse (T,C,F,D) template arguments
	if(tok_.id == TokenId::ROBracket)
	{
		next(); //skip '('

		while(tok_.id != TokenId::RCBracket)
		{
			check(TokenId::Ident);
			auto name = transfer(tok_.buffer);
			next(); //skip ident

			TypePtr type;
			if(tok_.id == TokenId::Colon)
			{
				next();
				type = parseDataType();
			}
			//structDecl->tpl_params.push_back(TemplateParameter(name, type));
			structDecl->tpl_params.emplace_back(name, type);

			if(tok_.id == TokenId::Comma)
				next();
		}
		check(TokenId::RCBracket);
		next();
	}

	//empty struct
	if(tok_.id == TokenId::Semicolon)
	{
		next();
		return structDecl;
	}

	//inherit
	if(tok_.id == TokenId::Colon)
	{
		next(); //skip ':'
		//parse pub/priv/prot flags?
		auto type = parseDataType();
		structDecl->inheritType = type;
	}

	//open { }
	check(TokenId::COBracket);

	next();

	while(tok_.id != TokenId::CCBracket)
	{
		//add a struct decl

		check(TokenId::Ident);
		auto name = transfer(tok_.buffer);
		checkNext(TokenId::Colon);
		next(); //skip ':'
		auto type = parseDataType();

		structDecl->fields.push_back(StructField(name, type));

		if(tok_.id == TokenId::Comma)
			next();
	}

	assert(tok_.id == TokenId::CCBracket);
	next();
	return structDecl;
}

DeclPtr Parser::parseEnumDecl()
{
	assert(tok_.id == TokenId::KwEnum);
	checkNext(TokenId::Ident);

	auto enm = Node::create<EnumDecl>();
	enm->name = transfer(tok_.buffer);
	next(); //skip name ident

	//basic underlying data type
	if(tok_.id == TokenId::Colon)
	{
		next(); //skip colon
		enm->basic_type = parseDataType();
	}

	//TODO allow empty enums?

	check(TokenId::COBracket);
	next();

	//parse fields
		//A
		//A = Expr
		//A(type,type)
		//A{name:type, .. }
	while(true)
	{
		if(tok_.id == TokenId::CCBracket)
			break;

		EnumField ef; //enum item?

		//field name
		check(TokenId::Ident);
		ef.name = transfer(tok_.buffer);
		next();

		//parse tuple field
		if(tok_.id == TokenId::ROBracket)
		{
			next();
			auto tuple = Node::create<TupleDecl>();

			//parse field contents
			while(true)
			{
				if(tok_.id == TokenId::RCBracket)
					break;

				auto type = parseDataType();
				tuple->fields.push_back(type);

				//check for other
				if(tok_.id == TokenId::Comma)
					next();
				else
					break;
			}

			ef.decl = tuple; //set decl to tuple decl

			check(TokenId::RCBracket);
			next();
		}

		//parse struct field
		if(tok_.id == TokenId::COBracket)
		{
			next();
			auto st = Node::create<StructDecl>();

			//parse struct contents
			while(true)
			{
				if(tok_.id == TokenId::CCBracket)
					break;

				check(TokenId::Ident);
				auto name = transfer(tok_.buffer);
				next();

				check(TokenId::Colon);
				next();

				auto type = parseDataType();

				//struct field
				st->fields.push_back(StructField(name, type));

				//check for other
				if(tok_.id == TokenId::Comma)
					next();
				else
					break;
			}
			ef.decl = st;
			check(TokenId::CCBracket);
			next();
		}

		//the enum field has a direct expression assigment
		if(tok_.id == TokenId::Assign)
		{
			next();
			ef.value = parseExpression();
		}

		//add final field
		enm->fields.emplace_back(ef);

		//check for other fields
		if(tok_.id == TokenId::Comma)
			next();
		else
			break;
	}
	check(TokenId::CCBracket);
	next(); //skip closing }

	return enm;
}

DeclPtr Parser::parseTypeDecl()
{
	assert(tok_.id == TokenId::KwType);

	throw plf::FormatException("Parsing type aliases not yet implemented");
}

/*
* var/let/const name [: type] [= init];
*/
plf::DeclPtr Parser::parseInstanceDecl(ParseOption p)
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
	
	if(p == ParseOption::CheckSemicolon)
	{
		check(TokenId::Semicolon);
		next(); //TODO here required for checking semicolon?
	}
	
	return inst;
}

AttrPtr Parser::parseAttribute()
{
	//multiple #![<attr>] -> parseAttributes

	throw Exception("Not yet implemented");
}

void Parser::parseTplParameter()
{
	//Ident : Type

}

////////////////////////////////////////////////////////////////////////
// Statements
////////////////////////////////////////////////////////////////////////

/**
* Parse statements
*/
StmtPtr Parser::parseStatement()
{
	/*if(tok_.id == TokenId::Eof)
		return;
	*/

	switch(tok_.id)
	{
		case TokenId::KwFor:
			return parseForStmt();
			break;
		case TokenId::KwWhile:
			return parseWhileStmt();
			break;
		case TokenId::COBracket:
			return parseBlockStmt();
			break;
		//syntax: unsafe { }
		case TokenId::KwUnsafe:
		{
			next();
			auto block = parseBlockStmt();
			block->to<BlockStmt>()->unsafe = true;
			return block;
		}
		case TokenId::KwReturn:
		{
			next();
			auto retstmt = Node::create<ReturnStmt>();
			if(tok_.id != TokenId::Semicolon)
			{
				retstmt->expr = parseExpression();
			}
			check(TokenId::Semicolon);
			next();
			return retstmt;
		}

		/* is an unary expression
		//handle mixins or static execution
		case TokenId::DollarDollar:
			throw Exception("parseStatement: Compile Time Execution stuff not yet implemented");

		//doing a peek here?
			// peek(1),
		*/

		default:
			break;
	}
	
	//try declaration parsing:
	// -> all declarations starts with a keyword
	auto decl = parseDeclaration();
	if(decl && decl->kind != NodeKind::Error)
	{
		auto declStmt = Node::create<DeclStmt>();
		
		//TODO check kinds for allowed DeclStmt???
		
		declStmt->decl = decl;
		return declStmt;
	}
	
	//try expression parsing
	auto expr = parseExpression();
	if(expr)
	{
		//after valid expression in statement scope semicolon is required
		//	to end the expr statement
		check(TokenId::Semicolon);
		next(); //skip ';'

		auto exprStmt = Node::create<ExprStmt>();
		exprStmt->expr = expr;
		return exprStmt;
	}
		
	//if nothing matched error:
	throw FormatException("parseStatement: Not yet fully implemented or invalid token [%s]", toString(tok_.id));
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

		//only expression; expression ...

		//TODO check error
		if(stmt->kind == NodeKind::Error)
		{
			std::cerr << "found error " << std::endl;
		}

		blockStmt->statements.push_back(stmt);
	}
	
	//skip close '}'
	assert(tok_.id == TokenId::CCBracket);
	next();
	
	return blockStmt;
}

/**
 * @brief Parser::parseForStmt
 *	for (<decl-stmt>, <cond-expr>, <stmt>)
 * @return for stmt
 */
StmtPtr Parser::parseForStmt()
{
	assert(tok_.id == TokenId::KwFor);
	next();

	auto forstmt = Node::create<ForStmt>();

	check(TokenId::ROBracket);
	next();

	//decl
	//TODO empty ; and multiple with ,
	//TODO pay attention for ; after global variable declaration O_o

	auto declstmt = Node::create<DeclStmt>();
	declstmt->decl = parseInstanceDecl(ParseOption::NoSemicolonCheck);
	forstmt->init = declstmt;

	check(TokenId::Semicolon);
	next();

	//cond-expr
	forstmt->cond = parseExpression();

	check(TokenId::Semicolon);
	next();

	//after loop stmt
	//TODO -> all expr statements require ; after?
	forstmt->after = parseExpression();

	check(TokenId::RCBracket); // closing )
	next();

	check(TokenId::COBracket);
	forstmt->body = parseBlockStmt();

	return forstmt;
}

/**
 * @brief Parser::parseWhileStmt
 * while(<cond-expr>) { }
 * @return a while stmt
 */
StmtPtr Parser::parseWhileStmt()
{
	assert(tok_.id == TokenId::KwWhile);
	next();

	auto whilestmt = Node::create<WhileStmt>();

	//condition
	whilestmt->cond = parseExpression();

	//body { }
	check(TokenId::COBracket);
	whilestmt->body = parseBlockStmt();

	return whilestmt;
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
	switch(tok_.id)
	{
		//---------------------------
		//keyword expressions
		case TokenId::KwIf:
			return parseIfExpr();

		case TokenId::KwMatch:
			return parseMatchExpr();
			
		default:
			return parseExprBinary(0); //TODO right start prec
	}
}

/**
 * @brief Parser::parseExprAtom
 * @return
 */
ExprPtr Parser::parseExprAtom()
{
	ExprPtr expr;

	//prefix unary op
	auto prefix_op = op_unary(tok_.id, true);
	if(prefix_op != UnaryOperator::NOP)
	{
		next();
		auto uexp = Node::create<UnaryExpr>();
		uexp->op = prefix_op;
		uexp->expr = parseExprAtom();
		return uexp;
	}

	//main stuff and other prefix stuff
	switch(tok_.id)
	{
		//special '(', <expr>, ')'
		case TokenId::ROBracket:
		{
			next();
			expr =  parseExprBinary(0); //TODO right start prec
			if(tok_.id != TokenId::RCBracket)
				throw FormatException("Missing )");
			next();
			return expr;
			break;
		}

		//MAIN====================================

		case TokenId::IntLiteral:
		{
			auto l = Node::create<IntegerLiteral>();
			l->rawValue = transfer(tok_.buffer);
			expr = l;
			next();
			break;
		}

		case TokenId::FloatLiteral:
		{
			auto l = Node::create<FloatLiteral>();
			l->rawValue = transfer(tok_.buffer);
			expr = l;
			next();
			break;
		}

		case TokenId::HexLiteral:
		{
			auto l = Node::create<HexLiteral>();
			l->rawValue = transfer(tok_.buffer);
			expr = l;
			next();
			break;
		}

		case TokenId::BinaryLiteral:
		{
			auto l = Node::create<BinaryLiteral>();
			l->rawValue = transfer(tok_.buffer);
			expr = l;
			next();
			break;
		}

		case TokenId::StringLiteral:
		{
			auto l = Node::create<StringLiteral>();
			l->rawValue = transfer(tok_.buffer);
			expr = l;
			next();
			break;
		}

		case TokenId::KwFalse:
		{
			auto l = Node::create<BoolLiteral>();
			l->value = false;
			expr = l;
			next();
			break;
		}

		case TokenId::KwTrue:
		{
			auto l = Node::create<BoolLiteral>();
			l->value = true;
			expr = l;
			next();
			break;
		}

		case TokenId::Ident:
		{
			auto id = Node::create<IdentExpr>();
			id->ident = transfer(tok_.buffer);
			expr = id;
			next();
			break;
		}

		case TokenId::KwDef:
		{
			//TODO can be clojure
			// def ( params ) = expr
			// def ( params ) { }

			auto func = parseFunctionDecl(true);
			auto expr = Node::create<LambdaExpr>();
			expr->func = func->to<FunctionDecl>();
			return expr;
		}

		/* already handled -> prefix unary expr?
		case TokenId::Dollar:
			//$Decl
			//$Source
			//$(expr).asdad
			throw Exception("parseExprAtom: compile time expressions not yet implemented");

		case TokenId::DollarDollar:
			throw Exception("parseExprAtom: compile time expressions not yet implemented");
		*/

		default: break;
	}


	//no expression at this stage == error
	if(!expr)
		throw FormatException("Not an expression [token: %s, line %d ]", toString(tok_.id), tok_.loc.line);

	//Postfix Unary Expr =======================================================
	auto postfix_op = op_unary(tok_.id, false);
	if(postfix_op != UnaryOperator::NOP)
	{
		next();
		auto uexp = Node::create<UnaryExpr>();
		uexp->op = postfix_op;
		uexp->expr = expr;
		return uexp;
	}


	//Call/ArrayAccess Operator
	switch(tok_.id)
	{
		// abc(i, 1, u)
		case TokenId::ROBracket:
		{
			next();
			auto callexpr = Node::create<CallExpr>();

			callexpr->decl_expr = expr;

			while(tok_.id != TokenId::RCBracket)
			{
				auto expr = parseExpression();
				callexpr->params.push_back(expr);
				if(tok_.id == TokenId::Comma)
					next();
			}
			check(TokenId::RCBracket);
			next(); //skip ')'

			return callexpr;
		}

		// abc[i]
		case TokenId::SOBracket:
			throw Exception("Array Index Expr not yet implemented");

		//! postfix for stuff like abc!i8(), abc!(i8)()
		case TokenId::EPoint:
			throw Exception("Template type instance expression not yet implemented");

		default: return expr;
	}

	//can not be reached
	throw FormatException("Not expected token: ", toString(tok_.id));
}

/**
 * @brief Parser::parseExprBinary
 * @param min_prec
 * @return
 */
ExprPtr Parser::parseExprBinary(int min_prec)
{
	ExprPtr left = parseExprAtom();

	while(true)
	{
		const BinaryOperator op = op_binary(tok_.id);

		if(tok_.id == TokenId::Eof		//end of file
		|| op == BinaryOperator::NOP	//not a binary operator
		|| op_prec(op) < min_prec)		//precedence is smaller
		break;

		//assert(is_bin_op(tok_.id), "no binop");

		//get associativity and min_prec
		const OpAssociativity assoc = op_assoc(op);
		int next_min_prec = (assoc == OpAssociativity::Left) ? op_prec(op) + 1 : op_prec(op);

		next(); //skip binary operator token

		//TODO look for as operator
		if(op == BinaryOperator::As)
		{
			throw Exception("Cast operator 'as' not implemented");
		}

		//look at right
		ExprPtr right = parseExprBinary(next_min_prec);

		//create final binary expression
		auto bin_expr = Node::create<BinaryExpr>();
		bin_expr->left = left;
		bin_expr->op = op;
		bin_expr->assoc = assoc;
		bin_expr->right = right;

		left = bin_expr;
	}
	return left;
}


/*
* if(<expr>){}
* else if(<expr>){}
* else{}
*/
ExprPtr Parser::parseIfExpr()
{
	assert(current(TokenId::KwIf));
	next();

	auto ifexpr = Node::create<IfExpr>();

	ifexpr->condition = parseExpression();
	ifexpr->condition->parent = ifexpr;


	check(TokenId::COBracket);
	ifexpr->stmt = parseBlockStmt();
	ifexpr->stmt->parent = ifexpr;

	if(tok_.id == TokenId::KwElse)
	{
		next(); //skip else

		//else block
		if(tok_.id == TokenId::COBracket)
		{
			ifexpr->tail = parseBlockStmt();
			return ifexpr;
		}

		//else if
		if(tok_.id == TokenId::KwIf)
		{
			auto stmt =  Node::create<ExprStmt>();
			stmt->expr =  parseIfExpr();
			ifexpr->tail = stmt;
			return ifexpr;
		}
	}

	return ifexpr;
}

/*
* match <expr> { }
*/
ExprPtr Parser::parseMatchExpr()
{
	assert(current(TokenId::KwMatch));
	next();

	auto match = Node::create<MatchExpr>();

	match->value = parseExpression();

	check(TokenId::COBracket);
	next();

	//parse pattern => {} or <expr>
	//TODO Match Syntax!!! -> case Syntax

	check(TokenId::CCBracket);
	next();

	throw Exception("Match-Expression parsing not yet implemented");
}

////////////////////////////////////////////////////////////////////////
// DataType
////////////////////////////////////////////////////////////////////////

TypePtr Parser::parseDataTypeSingle()
{
	//prefix
		//$
	if(current(TokenId::Dollar))
	{
		next(); //skip $
		auto type = Type::create<CTType>();
		type->type = parseDataTypeSingle();
		return type;
	}

	//prefix ::

	TypePtr type;

	//main:
	switch(tok_.id)
	{
		case TokenId::Ident:
		{
			type = checkForBuiltinType(tok_.buffer);
			next(); //skip ident
			break;
		}
	default: break;
	}


	//postfix
			//!type
			//!(type)
	switch(tok_.id)
	{
		case TokenId::EPoint:
		{
			throw Exception("error no tpl type instance parsing");
			break;
		}
	default:break;
	}


	//down here something has to been in type
	if(!type)
		throw FormatException("(%d, %d) parseDataTypeSingle: not yet implemented or invalid type (%s)",tok_.loc.line, tok_.loc.column, toString(tok_.id));

	return type;
}

/**
 * @brief Parser::parseDataTypeMulti
 *			parse type paths (similiar binary expr parsing)
 * @return TypePtr - ptr to data type
 */
TypePtr Parser::parseDataTypeMulti()
{
	TypePtr left = parseDataTypeSingle();

	while(true)
	{
		if(current(TokenId::ColonColon))
		{
			next(); //skip ::
			TypePtr right = parseDataTypeMulti();

			//TODO return path expression
			auto path = Type::create<PathType>();
			path->left = left;
			path->right = right;
			left = path;
		}
		else
			break;
	}

	return left;
}


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
	// function types can't be nested
	// def(type, type, ...) : type
	if(current( TokenId::KwDef))
	{
		checkNext(TokenId::ROBracket);
		next();

		auto func_type = Type::create<FunctionType>();

		//parse parameter
		while(true)
		{
			if(tok_.id == TokenId::RCBracket)
				break;

			auto type = parseDataType();
			func_type->params.push_back(type);

			if(tok_.id != TokenId::Comma)
				break;
			else
				next(); //skip comma
		}
		check(TokenId::RCBracket);
		next();

		//parse return type
		if(tok_.id == TokenId::Colon)
		{
			next();
			auto rettype = parseDataType();
			func_type->returnType = rettype;
		}
		return func_type;
	}
	

	//start tokens
	switch(tok_.id)
	{	
		//heap owned ptr type
		case TokenId::Tilde:
		{
			next();
			auto opt = Type::create<OwnedPtrType>();
			opt->targetType = parseDataTypeMulti();
			return opt;
		}
			
		//borrowed pointer
		case TokenId::And:
		{
			next();
			auto bpt = Type::create<BorrowedPtrType>();
			bpt->targetType =  parseDataTypeMulti();
			return bpt;
		}
			
		//raw unsafe pointer
		case TokenId::Mul:
		{
			next();
			auto rpt = Type::create<RawPtrType>();
			rpt->targetType =  parseDataTypeMulti();
			return rpt;
		}
		
		//also [] array types
		//arrays also alone standing?
		case TokenId::SOBracket:
		{
			next(); //skip over [

			auto type = Type::create<ArrayType>();

			//sized array
			if(tok_.id == TokenId::IntLiteral)
			{
				throw Exception("parseDataType: sized array parsing not fully implemented");
				//size hint
				type->size = 0;
				next();
			}

			check(TokenId::SCBracket);
			next(); //skip over ]

			auto datatype = parseDataTypeMulti(); //also step in?
			type->targetType = datatype;
			return type;
		}

		case TokenId::ConstraintStart:
			throw FormatException("parseDataType: parsing of type constraints not yet implemented");

		//default go into parseDataTypeMulti
		default:break;
	}

	return parseDataTypeMulti();
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

bool Parser::current(TokenId id)
{
	return tok_.id == id;
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

void Parser::warning(const char* msg, ...)
{
	static const int BUFSIZE=1024;
	char buffer[BUFSIZE];
	va_list argument_list;
	va_start(argument_list, msg);

	int len = vsnprintf(buffer, BUFSIZE - 2, msg, argument_list);
	if(len < 0 || len > BUFSIZE - 2)
	{
		len = BUFSIZE - 2;
	}
	buffer[len] = '\0';

	va_end(argument_list);

	std::cerr.write(buffer, len);
}

void Parser::error(const char* msg, ...)
{
	static const int BUFSIZE=1024;
	char buffer[BUFSIZE];
	va_list argument_list;
	va_start(argument_list, msg);

	int len = vsnprintf(buffer, BUFSIZE - 2, msg, argument_list);
	if(len < 0 || len > BUFSIZE - 2)
	{
		len = BUFSIZE - 2;
	}
	buffer[len] = '\0';

	va_end(argument_list);

	std::cerr.write(buffer, len);
}
