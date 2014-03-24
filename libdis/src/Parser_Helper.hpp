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


namespace {

#include <dis/Token.hpp>
#include <plf/ast/Expression.hpp>
using namespace dis;
using namespace plf;

//get unary operator for Token
UnaryOperator op_unary(TokenId id)
{
	switch(id)
	{
	default: return UnaryOperator::NOP;
	}

	throw Exception("not yet implemented");
}

//get binary operator for Token
BinaryOperator op_binary(TokenId id)
{
	switch(id)
	{
	default: return BinaryOperator::NOP;
	}

	throw Exception("not yet implemented");
}

//get assocs OpAssociativity

OpAssociativity op_assoc(BinaryOperator op)
{
	throw Exception("not yet implemented");
}

/// return unary operator precedence
int op_prec(plf::UnaryOperator& op)
{
	switch(op)
	{
	case UnaryOperator::NOP:
		throw Exception("Not an operator");
	default: throw Exception("not yet implemented");
	}
	throw Exception("not yet implemented");
}

/// return bin operator precedence
int op_prec(plf::BinaryOperator& op)
{
	switch(op)
	{
	case BinaryOperator::NOP:
		throw Exception("Not an operator");
	default: throw Exception("not yet implemented");
	}
	throw Exception("not yet implemented");
}



}
