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
#ifndef __PLF_STATEMENT_HPP__
#define __PLF_STATEMENT_HPP__

#include <plf/ast/Node.hpp>

namespace plf {

class Statement : public Node
{
};

//ScopeStmt all statements with seperate scope (for/while/dowhile/if/switch)
//BlockStmt : ScopeStatement

/**
* Return Statement
*/
class ReturnStmt : public Statement
{
public:
	ExprPtr expr;
};

/**
* Statement for 'for' loop
* for(init; cond; after)
*/
class ForStmt : public Statement
{
public:
	StmtPtr init;
	ExprPtr cond;
	StmtPtr after;
	//stmt
};

/**
* Statement for 'while' loop
* while(cond) stmt
*/
class WhileStmt : public Statement
{
public:
	ExprPtr cond;
	//stmt
};

/**
* Declaration Statement
*/
class DeclStmt : public Statement
{
public:
	DeclPtr decl;
};

/**
* Expression Statement
*/
class ExprStmt : public Statement
{
public:
	ExprPtr expr;
};



} //end namespace plf

#endif //__PLF_STATEMENT_HPP__
