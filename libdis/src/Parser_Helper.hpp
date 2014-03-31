/*
Dis Programming Language Frontend Library

Copyright (c) 2014 okard

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


#include <plf/base/Buffer.hpp>
#include <plf/ast/Expression.hpp>

#include <dis/Token.hpp>
using namespace plf;
using namespace dis;

namespace {

//get unary operator for Token
UnaryOperator op_unary(TokenId id, bool prefix)
{
	//Prefix
	if(prefix)
	switch(id)
	{
	case TokenId::Plus: return UnaryOperator::Pos;
	case TokenId::Minus: return UnaryOperator::Neg;
	case TokenId::Tilde: return UnaryOperator::OwnedPtr;
	case TokenId::PlusPlus: return UnaryOperator::PreIncr;
	case TokenId::MinusMinus: return UnaryOperator::PreDecr;
	case TokenId::And: return UnaryOperator::Ref;
	case TokenId::EPoint: return UnaryOperator::LNot;
	default: return UnaryOperator::NOP;
	}

	//Postfix
	if(!prefix)
	switch(id)
	{
	case TokenId::PlusPlus: return UnaryOperator::PostIncr;
	case TokenId::MinusMinus: return UnaryOperator::PostDecr;
	default: return UnaryOperator::NOP;
	}

	throw Exception("Not reachable");
}

//get binary operator for Token
BinaryOperator op_binary(TokenId id)
{
	switch(id)
	{
	case TokenId::Assign: return BinaryOperator::Assign;

	//arithmetic single
	case TokenId::Plus: return BinaryOperator::Plus;
	case TokenId::Minus: return BinaryOperator::Minus;
	case TokenId::Mul: return BinaryOperator::Mul;
	case TokenId::Div: return BinaryOperator::Div;
	case TokenId::Mod: return BinaryOperator::Mod;

	//arithmetic combound
	case TokenId::PlusAssign: return BinaryOperator::PlusAssign;

	//bitwise

	//Logical
	case TokenId::Equal: return BinaryOperator::Equal;
	case TokenId::NEqual: return BinaryOperator::NotEqual;
	case TokenId::Greater: return BinaryOperator::Gt;
	case TokenId::GTE:	return BinaryOperator::Gte;
	case TokenId::Less: return BinaryOperator::Lt;
	case TokenId::LTE: return BinaryOperator::Lte;

	//case TokenId::?????: return BinaryOperator::Concat;

	//Special
	case TokenId::Dot: return BinaryOperator::Access;
	case TokenId::KwAs: return BinaryOperator::As;

	// Non operator
	default: return BinaryOperator::NOP;
	}
}

//get assocs OpAssociativity
OpAssociativity op_assoc(BinaryOperator op)
{
	switch(op)
	{
	case BinaryOperator::Access: return OpAssociativity::Left;

	case BinaryOperator::Plus: return OpAssociativity::Left;

	case BinaryOperator::Assign: return OpAssociativity::Right;

	default: throw Exception("op_assoc: not yet implemented");
	}
}

/// return unary operator precedence
int op_prec(plf::UnaryOperator& op)
{
	switch(op)
	{
	case UnaryOperator::Neg:
	case UnaryOperator::Pos:
		return 3;
	case UnaryOperator::NOP: throw Exception("Not an operator");
	default: throw Exception("op_prec: not yet implemented");
	}
}

/// return bin operator precedence
int op_prec(plf::BinaryOperator& op)
{
	switch(op)
	{
	case BinaryOperator::Access:
		return 2;

	case BinaryOperator::Mul:
	case BinaryOperator::Div:
	case BinaryOperator::Mod:
		return 5;

	case BinaryOperator::Plus:
	case BinaryOperator::Minus:
		return 6;

	case BinaryOperator::Gt:
	case BinaryOperator::Gte:
	case BinaryOperator::Lt:
	case BinaryOperator::Lte:
		return 8;
	case BinaryOperator::Equal:
	case BinaryOperator::NotEqual:
		return 9;

	case BinaryOperator::LAnd:
		return 10;
	case BinaryOperator::LOr:
		return 11;

	case BinaryOperator::Assign:
	case BinaryOperator::PlusAssign:
		return 12;

	//logical:

	case BinaryOperator::NOP:  throw Exception("Not an operator");
	default: throw Exception("op_prec: not yet implemented");
	}
}

//transfer a buffer ptr
BufferPtr transfer(BufferPtr& bufptr)
{
	auto p = bufptr;
	bufptr = std::make_shared<Buffer>();
	return p;
}

}
