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
#include <plf/base/Exception.hpp>
#include <plf/ast/Expression.hpp>

#include <dis/Token.hpp>

#include <unordered_map>

using namespace plf;
using namespace dis;


#include "Helper.hpp"

namespace {

//get unary operator for Token
//TODO split to postfix/prefix functions?
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

	case TokenId::Dollar: return UnaryOperator::CTAccess;
	case TokenId::DollarDollar: return UnaryOperator::CTCall;

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

/*

/// return unary operator precedence
int op_prec(plf::UnaryOperator& op)
{
	switch(op)
	{
	case UnaryOperator::Neg:
	case UnaryOperator::Pos:
		return 3;
	case UnaryOperator::NOP: throw Exception("Not an operator");
	default: throw Exception("op_prec(unary): not yet implemented");
	}
}
*/



//get binary operator for Token
BinaryOperator op_binary(TokenId id)
{
	switch(id)
	{
	case TokenId::ColonColon: return BinaryOperator::AccessPath;
	case TokenId::Assign: return BinaryOperator::Assign;

	//arithmetic single
	case TokenId::Plus: return BinaryOperator::Plus;
	case TokenId::Minus: return BinaryOperator::Minus;
	case TokenId::Mul: return BinaryOperator::Mul;
	case TokenId::Div: return BinaryOperator::Div;
	case TokenId::Mod: return BinaryOperator::Mod;

	//arithmetic combound
	case TokenId::PlusAssign: return BinaryOperator::PlusAssign;
	case TokenId::MinusAssign: return BinaryOperator::MinusAssign;
	case TokenId::MulAssign: return BinaryOperator::MulAssign;
	case TokenId::DivAssign: return BinaryOperator::DivAssign;
	case TokenId::ModAssign: return BinaryOperator::ModAssign;

	//bitwise
	case TokenId::ShiftL: return BinaryOperator::ShiftL;
	case TokenId::ShiftR: return BinaryOperator::ShiftR;

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
	//case BinaryOperator::AccessPath: return OpAssociativity::Right;
	case BinaryOperator::Assign: return OpAssociativity::Right;

	//Arithmetic
	case BinaryOperator::Plus: return OpAssociativity::Left;
	case BinaryOperator::Minus: return OpAssociativity::Left;
	case BinaryOperator::Mul: return OpAssociativity::Left;
	case BinaryOperator::Div: return OpAssociativity::Left;
	case BinaryOperator::Mod: return OpAssociativity::Left;
	case BinaryOperator::Pow: return OpAssociativity::Left;

	//Arithmetic Compound Assign
	case BinaryOperator::PlusAssign: return OpAssociativity::Right;
	case BinaryOperator::MinusAssign: return OpAssociativity::Right;
	case BinaryOperator::MulAssign: return OpAssociativity::Right;
	case BinaryOperator::DivAssign: return OpAssociativity::Right;
	case BinaryOperator::ModAssign: return OpAssociativity::Right;
	case BinaryOperator::PowAssign: return OpAssociativity::Right;

	//bitwise
	case BinaryOperator::ShiftL: return OpAssociativity::Left;
	case BinaryOperator::ShiftR: return OpAssociativity::Left;

	//greater, less comparison
	case BinaryOperator::Gt: return OpAssociativity::Left;
	case BinaryOperator::Gte: return OpAssociativity::Left;
	case BinaryOperator::Lt: return OpAssociativity::Left;
	case BinaryOperator::Lte: return OpAssociativity::Left;


	//Other:
	case BinaryOperator::Access: return OpAssociativity::Left;

	default: throw Exception("op_assoc(binary): not yet implemented");
	}
}

/// return bin operator precedence
int op_prec(plf::BinaryOperator op)
{
	switch(op)
	{
	case BinaryOperator::AccessPath:
		return 1;

	case BinaryOperator::Access:
		return 2;

	case BinaryOperator::Mul:
	case BinaryOperator::Div:
	case BinaryOperator::Mod:
		return 5;

	case BinaryOperator::Plus:
	case BinaryOperator::Minus:
		return 6;

	case BinaryOperator::ShiftL:
	case BinaryOperator::ShiftR:
		return 7;

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
	case BinaryOperator::MinusAssign:
	case BinaryOperator::MulAssign:
	case BinaryOperator::DivAssign:
	case BinaryOperator::ModAssign:
		return 12;

	//logical:

	case BinaryOperator::NOP:  throw Exception("Not an operator");
	default: throw Exception("op_prec(binary): not yet implemented");
	}
}

//transfer a buffer ptr
BufferPtr transfer(BufferPtr& bufptr)
{
	auto p = bufptr;
	bufptr = std::make_shared<Buffer>();
	return p;
}


//quickly resolve builtin types
inline static TypePtr checkForBuiltinType(BufferPtr& buf)
{
	static const std::unordered_map<std::size_t, TypePtr, hash_dummy> keywordMap =
	{
		{ const_hash("i8"), PrimaryType::TypeI8() },
		{ const_hash("u8"), PrimaryType::TypeU8() },
		{ const_hash("i16"), PrimaryType::TypeI16() },
		{ const_hash("u16"), PrimaryType::TypeU16() },
		{ const_hash("i32"), PrimaryType::TypeI32() },
		{ const_hash("u32"), PrimaryType::TypeU32() },
		{ const_hash("i64"), PrimaryType::TypeI64() },
		{ const_hash("u64"), PrimaryType::TypeU64() }
		//f32
		//f64
	};

	auto it = keywordMap.find(hash(buf->ptr(), buf->size()));
	if(it != keywordMap.end())
	{
		return it->second;
	}
	else
	{
		auto type = Type::create<UnsolvedType>();
		type->idents.push_back(transfer(buf));
		return type;
	}
}


}
