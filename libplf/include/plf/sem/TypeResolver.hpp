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
#ifndef __PLF_TYPERESOLVER_HPP__
#define __PLF_TYPERESOLVER_HPP__

#include <plf/ast/Visitor.hpp>
#include <plf/ast/Crate.hpp>

namespace plf {

/**
* Semantic Pass to resolve types
*/
class TypeResolver : public DispatchVisitor<NodePtr, Ref>
{
private:
	//private statistics
	//unresolved stuff (queue?)
	//etc
	//status of interpreter requirement?

public:

	//missed dispatch
	virtual NodePtr visit(Node& n);

	//Declarations------------------------------------------------------
	virtual NodePtr visit(ModDecl& n);
	virtual NodePtr visit(UseDecl& n);
	virtual NodePtr visit(ClassDecl& n);
	virtual NodePtr visit(TraitDecl& n);
	virtual NodePtr visit(StructDecl& n);

	virtual NodePtr visit(EnumDecl& n);
	virtual NodePtr visit(AliasDecl& n);
	virtual NodePtr visit(FunctionDecl& n);
	virtual NodePtr visit(InstanceDecl& n);


	//Statements--------------------------------------------------------
	virtual NodePtr visit(BlockStmt& n);
	virtual NodePtr visit(ReturnStmt& n);
	virtual NodePtr visit(ForStmt& n);
	virtual NodePtr visit(WhileStmt& n);
	virtual NodePtr visit(DeclStmt& n);
	virtual NodePtr visit(ExprStmt& n);

	//Expressions-------------------------------------------------------
	virtual NodePtr visit(IntegerLiteral& n);
	virtual NodePtr visit(FloatLiteral& n);
	virtual NodePtr visit(HexLiteral& n);
	virtual NodePtr visit(BinaryLiteral& n);
	virtual NodePtr visit(StringLiteral& n);


	virtual NodePtr visit(IdentExpr& n);
	virtual NodePtr visit(UnaryExpr& n);
	virtual NodePtr visit(BinaryExpr& n);
	virtual NodePtr visit(CallExpr& n);


	//run type solver
	void run(Crate& crate);


	//call expressions
	//variable
	//const
	//values
};	
	
}
#endif 
