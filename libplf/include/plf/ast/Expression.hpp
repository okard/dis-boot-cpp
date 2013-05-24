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

namespace plf {
	
class Expression : public Node
{
public:
	//return type
};


enum class UnaryOperator
{
	Neg, 
	Not, 
	PostIncr, 
	PreIncr, 
	PostDecr, 
	PreDecr
};

class UnaryExpr : public Expression
{

public:
	UnaryOperator Op;
	ExprPtr Expr;
	
};

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
};

class BinaryExpr : public Expression
{
public:
	BinaryOperator Op;
	ExprPtr Left;
	ExprPtr Right;
};



//pathexpression


} //end namespace plf

#endif //__PLF_EXPRESSION_HPP__
