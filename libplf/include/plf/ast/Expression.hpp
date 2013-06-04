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
#ifndef __PLF_EXPRESSION_HPP__
#define __PLF_EXPRESSION_HPP__

#include <plf/ast/Node.hpp>
#include <plf/ast/Visitor.hpp>
#include <plf/ast/Type.hpp>

namespace plf {
	
class Expression : public Node
{
public:
	Expression(const NodeKind kind) : Node(kind) {}

	TypePtr returnType;
	
	//constant expression
};

///Operators for Unary Expressions
enum class UnaryOperator
{
	Neg, 
	Not, 
	PostIncr, 
	PreIncr, 
	PostDecr, 
	PreDecr
	
	// ~, &, @ for memory types?
};

/**
* Unary Expression
*/
class UnaryExpr : public Expression
{
public:
	static const NodeKind Kind = NodeKind::UnaryExpr;
	UnaryExpr() : Expression(NodeKind::UnaryExpr) {}
	inline NodePtr accept(Visitor& v, ParamPtr& arg) final { return v.visit(*this, arg); }
	
	UnaryOperator op;
	ExprPtr expr;
};

///Operators for Binary Expressions
enum class BinaryOperator
{
	Plus,
	Minus,
	Mul,
	Div,
	Mod,
	Equal,
	NotEqual,
	Assign,
	Gt, 	//GreaterThan
	Lt, 	//LessThan
	Gte, 	//GreaterThan or Equals 
	Lte 	// LessThan or Equals
};

/**
* Binary Expression
*/
class BinaryExpr : public Expression
{
public:
	static const NodeKind Kind = NodeKind::BinaryExpr;
	BinaryExpr() : Expression(NodeKind::BinaryExpr) {}
	inline NodePtr accept(Visitor& v, ParamPtr& arg) final { return v.visit(*this, arg); }
	
	BinaryOperator op;
	ExprPtr left;
	ExprPtr right;
};

/**
* A call expression
*/
class CallExpr final : public Expression
{
public:
	static const NodeKind Kind = NodeKind::CallExpr;
	CallExpr() : Expression(NodeKind::CallExpr) {}
	inline NodePtr accept(Visitor& v, ParamPtr& arg) final { return v.visit(*this, arg); }
	
	DeclPtr func;		//funcdecl
	ExprPtr klass; 		//instance expr
	ExprList params;	//instance expr
};

//pathexpression


} //end namespace plf

#endif //__PLF_EXPRESSION_HPP__
