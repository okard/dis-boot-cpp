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

#ifndef DIS_PARSER_ERROR
#define DIS_PARSER_ERROR

#include <plf/ast/Node.hpp>
#include <plf/ast/Declaration.hpp>

namespace dis {

//TODO generic ErrorInterface in plf/ast/Node.hpp?
//TODO clear error interface

//Cases of ErrorNodes
// - Invalid Decl
// - Invalid Stmt
// - Invalid Expr
// - EOF (is not an error -> seperate nodekind?)

//Properties
// - Message?
// - Accepted  (expected 'errors' so it is fine)
// - Fail	   (stop parsing)
// - Error Count ( count of errors in this parsing step?)

//handle error nodes through an signal(&Node);
//when one slot handler of signal changes attributes end parsing?)


class EofNode : public plf::Node
{
public:
	static const plf::NodeKind Kind = plf::NodeKind::Error;
	EofNode()
		: plf::Node(Kind)
	{
	}

	virtual plf::NodePtr accept(plf::Visitor&, plf::ParamPtr&) { throw "Not visitable";}
};



/**
* Error Result from parsing function
*/
class ErrorDecl : public plf::Declaration
{
public:
	static const plf::NodeKind Kind = plf::NodeKind::Error;

	ErrorDecl()
		: plf::Declaration(Kind)
	{}

	ErrorDecl(bool eof)
		: plf::Declaration(Kind), eof(eof)
	{}

	virtual plf::NodePtr accept(plf::Visitor&, plf::ParamPtr&) {throw "Not visitable";}

	bool eof = false;
	unsigned int col;
	unsigned int line;
	TokenId id;

	//EOF
	//Location
	//Message
	//Token?
	//type? Decl/Stmt/Expr/Type

	//in context flag? -> so current token hangs in air and for future parsing requires syncing

	//print function?
};

//errorstmt
//errorexpr


}

#endif // DIS_PARSER_ERROR
