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
#ifndef PLF_NODE_HPP
#define PLF_NODE_HPP

#include <plf/base/External.hpp>

namespace plf {
	
// Visitor Parameter Pointer
typedef AnyPtr ParamPtr;

//forward declaration
class Visitor;

class Node;
typedef SharedPtr<Node> NodePtr;
typedef WeakPtr<Node> WNodePtr;
typedef List<NodePtr> NodeList;

template<class T>
using PtrList = List<SharedPtr<T>>;

class Declaration;
typedef SharedPtr<Declaration> DeclPtr;
typedef List<DeclPtr> DeclList;

class Statement;
typedef SharedPtr<Statement> StmtPtr;
typedef List<StmtPtr> StmtList;

class Expression;
typedef SharedPtr<Expression> ExprPtr;
typedef List<ExprPtr> ExprList;

class Attribute;
typedef SharedPtr<Attribute> AttrPtr;
typedef List<AttrPtr> AttrList;

class Type;
typedef SharedPtr<Type> TypePtr;

/**
* Type of Ast Nodes
*/
enum class NodeKind : unsigned short
{
	Unknown,

	//Declarations------------------------------------------------------
	ModDecl,
	UseDecl,
	//-Type Decl
	FunctionDecl,
	ClassDecl,
	TraitDecl,
	StructDecl,
	AliasDecl,
	EnumDecl,
	//-Instance Decl
	InstanceDecl,
	
	//Statement---------------------------------------------------------
	BlockStmt,
	ReturnStmt,
	ForStmt,
	WhileStmt,
	ExprStmt,
	DeclStmt,
	
	//Expression--------------------------------------------------------
	IntegerLiteral,
	FloatLiteral,
	HexLiteral,
	BinaryLiteral,
	StringLiteral,

	IdentExpr,
	UnaryExpr,
	BinaryExpr,
	TernaryExpr,
	CallExpr,
	CastExpr,

	//Type--------------------------------------------------------------
	PrimaryType,
	UnkownType,
	DeclType,
	UnsolvedType,
	//-Pointer Types
	OwnedPtrType,
	BorrowedPtrType,
	RawPtrType,
	//PtrType,
	//RefType,

	//-RT Types:
	ArrayType,
	MapType,
	
	//Special-----------------------------------------------------------
	
	Error		//custom type for handling errors
	//ErrorDecl, ErrorStmt, ErrorExpr, 
};	

/**
* Default Ast Node
*/
class Node : public std::enable_shared_from_this<Node>
{
public:
	//kind of actual node
	const NodeKind kind;

	//weak ptr to parent
	WNodePtr parent;

public:
	//Node();
	Node(const NodeKind kind);
	virtual ~Node();
	
	
	/**
	* The Visitor Function
	*/
	virtual NodePtr accept(Visitor& v, ParamPtr& arg) = 0;
	
	/**
	* using for visitor return?
	*/
	inline operator NodePtr () { return shared_from_this(); }
	
	
	//TODO MemoryPool for Node Allocation?
	// void* operator new(size_t);
    // void operator delete(void*);
    
    //for template copies
    //NodePtr clone();
	
	/**
	* Cast helper
	*/
	template<class T>
	SharedPtr<T> to()
	{
		//debug dynamic cast
		#ifndef NDEBUG
			return std::dynamic_pointer_cast<T>(shared_from_this());
		#else
			return std::static_pointer_cast<T>(shared_from_this());
		#endif
	}
	
	/**
	* Create helper
	*/
	template<class T, typename... Args>
	static SharedPtr<T> create(Args... args)
	{
		NodeKind k = T::Kind; //make shure each node has it?
		k=k; //avoid warning
		return std::make_shared<T>(args...);
	}	
};
	
	
	
} //end namespace plf



#endif //PLF_NODE_HPP
