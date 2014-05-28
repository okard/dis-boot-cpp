/*
Programming Language Framework (PLF)

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
#ifndef PLF_STATEMENT_HPP
#define PLF_STATEMENT_HPP

#include <plf/ast/Node.hpp>
#include <plf/ast/Visitor.hpp>

namespace plf {

class Statement : public Node
{
public:
	Statement(const NodeKind kind) : Node(kind) {}
};

//ScopeStmt all statements with seperate scope (for/while/dowhile/if/switch)
//BlockStmt : ScopeStatement

/**
* Block of Statements
*/
class BlockStmt : public Statement
{
public:
	static const NodeKind Kind = NodeKind::BlockStmt;
	BlockStmt() : Statement(Kind) {}
	
	bool unsafe;
	StmtList statements;
};

/**
* Return Statement
*/
class ReturnStmt : public Statement
{
public:
	static const NodeKind Kind = NodeKind::ReturnStmt;
	ReturnStmt() : Statement(Kind) {}

	ExprPtr expr;
};


//if stmt
class IfStmt final : public Statement
{
public:
	static const NodeKind Kind = NodeKind::IfStmt;
	IfStmt() : Statement(Kind) {}

	ExprPtr condition;
	StmtPtr stmt;

	StmtPtr tail;
};

//match stmt
class MatchStmt final : public Statement
{
public:
	static const NodeKind Kind = NodeKind::MatchStmt;
	MatchStmt() : Statement(Kind) {}

	ExprPtr value;

	//Pattern?
	//format of conditions?
};


/**
* Statement for 'for' loop
* for(init; cond; after)
*/
class ForStmt : public Statement
{
public:
	static const NodeKind Kind = NodeKind::ForStmt;
	ForStmt() : Statement(Kind) {}
	
	StmtPtr init;		//multiple? with , seperated?
	ExprPtr cond;		//condition
	ExprPtr after;		//do something after loop
	StmtPtr body;
};

/**
* Statement for 'while' loop
* while(cond) stmt
*/
class WhileStmt : public Statement
{
public:
	static const NodeKind Kind = NodeKind::WhileStmt;
	WhileStmt() : Statement(Kind) {}
	
	ExprPtr cond;
	StmtPtr body;
	bool headCtr; //while(bool){} / do{}while(bool);
	//stmt
};

/**
* Declaration Statement
*/
class DeclStmt : public Statement
{
public:
	static const NodeKind Kind = NodeKind::DeclStmt;
	DeclStmt() : Statement(Kind) {}
	
	DeclPtr decl;
};

/**
* Expression Statement
*/
class ExprStmt : public Statement
{
public:
	static const NodeKind Kind = NodeKind::ExprStmt;
	ExprStmt() : Statement(Kind) {}
	
	ExprPtr expr;
};


} //end namespace plf

#endif //PLF_STATEMENT_HPP
