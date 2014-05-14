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
#ifndef PLF_EXPRESSION_HPP
#define PLF_EXPRESSION_HPP

#include <plf/ast/Node.hpp>
#include <plf/ast/Visitor.hpp>
#include <plf/ast/Type.hpp>

namespace plf {
	
/**
* Basic class for Expressions
*/
class Expression : public Node
{
public:
	Expression(const NodeKind kind) : Node(kind) {}

	TypePtr returnType;
	
	//constant expression
	
	//cte - compile time expression
};


//Literal Expressions:

//IntegerLiteral
class IntegerLiteral : public Expression
{
public:
	static const NodeKind Kind = NodeKind::IntegerLiteral;
	IntegerLiteral() : Expression(Kind) {}
	
	BufferPtr rawValue;
	
	//union
	//long long
	int value;
};

//FloatLiteral
class FloatLiteral : public Expression
{
public:
	static const NodeKind Kind = NodeKind::FloatLiteral;
	FloatLiteral() : Expression(Kind) {}
	
	BufferPtr rawValue;
};

//HexLiteral
class HexLiteral : public Expression
{
public:
	static const NodeKind Kind = NodeKind::HexLiteral;
	HexLiteral() : Expression(Kind) {}
	
	BufferPtr rawValue;
};

//BinaryLiteral
class BinaryLiteral : public Expression
{
public:
	static const NodeKind Kind = NodeKind::BinaryLiteral;
	BinaryLiteral() : Expression(Kind) {}
	
	BufferPtr rawValue;
};

//StringLiteral
class StringLiteral final : public Expression
{
public:
	static const NodeKind Kind = NodeKind::StringLiteral;
	StringLiteral() : Expression(Kind) {}
	
	BufferPtr rawValue;
	//Encoding?
};

/**
 * @brief The BoolLiteral class
 */
class BoolLiteral final : public Expression
{
public:
	static const NodeKind Kind = NodeKind::BoolLiteral;
	BoolLiteral() : Expression(Kind) {}

	bool value;
};


///Ident Expression
class IdentExpr : public Expression
{
public:
	static const NodeKind Kind = NodeKind::IdentExpr;
	IdentExpr() : Expression(Kind) {}

	BufferPtr ident;
};



//op assoc
enum class OpAssociativity
{
	Left,
	Right
};

///Operators for Unary Expressions
enum class UnaryOperator
{
	NOP, // no operator

	//Logical
	LNot, 	  // ! logical negative

	//Bitwise Not
	BNot,	  //

	//Arithmetic
	Neg, 	  // Prefix -
	Pos,	  // Prefix +
	PostIncr, //expr++
	PreIncr,  //++expr
	PostDecr, //expr--
	PreDecr,  //--expr
	
	//Memory
	Ref,	  // &
	OwnedPtr  // ~
	
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
	
	OpAssociativity assoc = OpAssociativity::Left;
	UnaryOperator op;
	ExprPtr expr;
};

///Operators for Binary Expressions
enum class BinaryOperator
{
	NOP,	//No operator

	Assign,	// =

	//Arithmetic
	Plus,	// +
	Minus,	// -
	Mul,	// *
	Div,	// /
	Mod,	// %
	Pow,	// ^^?

	//Arithmetic Compound Assign
	PlusAssign,		// +=
	MinusAssign,	// -=
	MulAssign,		// *=
	DivAssign,		// /=
	ModAssign,		// %=
	PowAssign,		// ^^=?

	//boolean operators
	Equal,		// ==
	NotEqual,	// !=
	Gt,			// GreaterThan >
	Lt,			// LessThan <
	Gte,		// GreaterThan or Equals >=
	Lte,		// LessThan or Equals <=
	LAnd,		// && Logic And
	LOr,		// || Logic Or

	//Bitwise
	And,		// &
	Or,			// |
	Xor,		// ^?
	ShiftL,		// <<
	ShiftR,		// >>

	//Bitwise Compound Assign
	AndAssign,
	OrAssign,
	XorAssign,
	ShiftLAssign,
	ShiftRAssign,

	//Other:
	Concat,			// ???? ~ ' #
	Access,			// .  foo.a(asad)
	AccessPath,		// ::

	//Comma Operator?

	As //Cast operator
};

/**
* Binary Expression
*/
class BinaryExpr : public Expression
{
public:
	static const NodeKind Kind = NodeKind::BinaryExpr;
	BinaryExpr() : Expression(NodeKind::BinaryExpr) {}
	
	OpAssociativity assoc = OpAssociativity::Left;
	BinaryOperator op;
	ExprPtr left;
	ExprPtr right;
};


//if
//match expr

/**
* A call expression
*/
class CallExpr final : public Expression
{
public:
	static const NodeKind Kind = NodeKind::CallExpr;
	CallExpr() : Expression(Kind) {}
	
	DeclPtr func;		//func decl / callable declaration
	ExprPtr decl_expr;	//expr to find decl
	ExprList params;	//call parameter
	
	//is ctcall 		//compile time call

};

/**
* Cast Expr
* <expr> as <type>
*/
class CastExpr final : public Expression
{
public:
	static const NodeKind Kind = NodeKind::CastExpr;
	CastExpr() : Expression(Kind) {}
	
	ExprPtr left;
	TypePtr type;
};

/**
 * @brief The LambdaExpr class
 */
class LambdaExpr final : public Expression
{
public:
	static const NodeKind Kind = NodeKind::LambdaExpr;
	LambdaExpr() : Expression(Kind) {}

	SharedPtr<FunctionDecl> func;
};



//IfExpr
//MatchExpr

//pathexpression


} //end namespace plf

#endif //PLF_EXPRESSION_HPP
