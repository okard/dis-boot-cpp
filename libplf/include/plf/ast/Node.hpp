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
#ifndef __PLF_NODE_HPP__
#define __PLF_NODE_HPP__

#include <plf/base/External.hpp>

namespace plf {
	
// Visitor Parameter Pointer
typedef AnyPtr ParamPtr;

//forward declaration
class Visitor;

class Node;
typedef SharedPtr<Node> NodePtr;
typedef List<NodePtr> NodeList;

class Declaration;
typedef SharedPtr<Declaration> DeclPtr;
typedef List<DeclPtr> DeclList;

class Statement;
typedef SharedPtr<Statement> StmtPtr;
typedef List<StmtPtr> StmtList;

class Expression;
typedef SharedPtr<Expression> ExprPtr;

/**
* Type of Ast Nodes
*/
enum class NodeKind : unsigned short
{
	//Declarations
	PackageDecl,
	ImportDecl,
	
	FunctionDecl,
	ClassDecl,
	VariableDecl,
	ValueDecl,
	ConstantDecl,
	AliasDecl,
	EnumDecl,
	StructDecl,
	
	//Type
	PrimaryType,
	PtrType,
	RefType,
	UserType, 		//user declared type
	//RT Types
	ArrayType,
	MapType,
	
	//Statement
	ReturnStmt,
	ForStmt,
	WhileStmt,
	ExprStmt,
	
	//Expression
	UnaryExpr,
	BinaryExpr,
	CallExpr,
	
	CTCallExpr, //compile time call expression $$name()
};	

/**
* Default Ast Node
*/
class Node : public std::enable_shared_from_this<Node>
{
private:
	NodeKind kind_; //const
	
public:

	Node();
	Node(const NodeKind nk);
	virtual ~Node();
	
	/**
	* Return Node Type
	*/
	inline NodeKind Kind() const { return kind_; }
	
	//virtual print(Stream)
	
	/**
	* The Visitor Function
	*/
	virtual NodePtr accept(Visitor& v, ParamPtr& arg);
	
	/**
	* using for visitor return?
	*/
	operator NodePtr () { return shared_from_this(); }
	
};
	
	
} //end namespace plf



#endif
