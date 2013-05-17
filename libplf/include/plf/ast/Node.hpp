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

#include <memory>
#include <vector>

#include <culcore/AnyPtr.hpp>

namespace plf {
	
// Visitor Parameter Pointer
typedef cul::AnyPtr ParamPtr;
	
//forward declaration
class Visitor;
class Node;
typedef std::shared_ptr<Node> NodePtr;
typedef std::vector<NodePtr> NodeList;

class Declaration;
typedef std::shared_ptr<Declaration> DeclPtr;
typedef std::vector<DeclPtr> DeclList;

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
	
	
	//Statement
	ReturnStmt,
	ForStmt,
	WhileStmt,
	DoWhileStmt, //while stmt?
	
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
	* helper template
	*/
	template<class V, class T>
	inline NodePtr accept_(V& v, ParamPtr& arg)
	{
		//if(T::Kind == this->Kind())
		auto o = std::static_pointer_cast<T>(shared_from_this());
		return v.visit(o, arg); 
	}
};
	
	
} //end namespace plf



#endif
