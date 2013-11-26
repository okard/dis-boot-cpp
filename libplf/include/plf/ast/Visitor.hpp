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
#ifndef __PLF_VISITOR_HPP__
#define __PLF_VISITOR_HPP__

#include <memory>

#include <plf/ast/Node.hpp>

namespace plf {
	
//Decl
class PackageDecl;
class ImportDecl;
class ClassDecl;
class TraitDecl;
class StructDecl;
class EnumDecl;
class AliasDecl;
class FunctionDecl;
class VariableDecl;
class ValueDecl;
class ConstDecl;

//Stmt
class BlockStmt;
class ReturnStmt;
class ForStmt;
class WhileStmt;
class DeclStmt;
class ExprStmt; 

//Expr
class IntegerLiteral;
class FloatLiteral;
class HexLiteral;
class BinaryLiteral;
class StringLiteral;

class UnaryExpr;
class BinaryExpr;
class CallExpr;	


/**
* Ast Visitor Class
*/
class Visitor
{

public:	

	//basic case
	virtual NodePtr visit(Node& n, ParamPtr& arg);

	//basic node types
	virtual NodePtr visit(Declaration& n, ParamPtr& arg);
	virtual NodePtr visit(Statement& n, ParamPtr& arg);
	virtual NodePtr visit(Expression& n, ParamPtr& arg);
	 
	//Declarations------------------------------------------------------
	virtual NodePtr visit(PackageDecl& n, ParamPtr& arg);
	virtual NodePtr visit(ImportDecl& n, ParamPtr& arg);
	virtual NodePtr visit(ClassDecl& n, ParamPtr& arg);
	virtual NodePtr visit(TraitDecl& n, ParamPtr& arg);
	virtual NodePtr visit(StructDecl& n, ParamPtr& arg);
	
	virtual NodePtr visit(EnumDecl& n, ParamPtr& arg);
	virtual NodePtr visit(AliasDecl& n, ParamPtr& arg);
	virtual NodePtr visit(FunctionDecl& n, ParamPtr& arg);
	virtual NodePtr visit(VariableDecl& n, ParamPtr& arg);
	virtual NodePtr visit(ValueDecl& n, ParamPtr& arg);
	virtual NodePtr visit(ConstDecl& n, ParamPtr& arg);
	
	
	//Statements--------------------------------------------------------
	virtual NodePtr visit(BlockStmt& n, ParamPtr& arg);
	virtual NodePtr visit(ReturnStmt& n, ParamPtr& arg);
	virtual NodePtr visit(ForStmt& n, ParamPtr& arg);
	virtual NodePtr visit(WhileStmt& n, ParamPtr& arg);
	virtual NodePtr visit(DeclStmt& n, ParamPtr& arg);
	virtual NodePtr visit(ExprStmt& n, ParamPtr& arg);
	
	//Expressions-------------------------------------------------------
	virtual NodePtr visit(IntegerLiteral& n, ParamPtr& arg);
	virtual NodePtr visit(FloatLiteral& n, ParamPtr& arg);
	virtual NodePtr visit(HexLiteral& n, ParamPtr& arg);
	virtual NodePtr visit(BinaryLiteral& n, ParamPtr& arg);
	virtual NodePtr visit(StringLiteral& n, ParamPtr& arg);
	
	virtual NodePtr visit(UnaryExpr& n, ParamPtr& arg);
	virtual NodePtr visit(BinaryExpr& n, ParamPtr& arg);
	virtual NodePtr visit(CallExpr& n, ParamPtr& arg);
	
	//UnaryExpr
		//SubUnaryExpressions
	//BinaryExpr
		//SubBinaryExpressions
	//CallExpr
	
	//Types-------------------------------------------------------------
};


} //end namespace plf

#endif // __PLF_VISITOR_HPP__
