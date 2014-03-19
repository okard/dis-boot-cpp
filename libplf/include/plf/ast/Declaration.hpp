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
#ifndef __PLF_DECLARATION_HPP__
#define __PLF_DECLARATION_HPP__

#include <plf/base/Buffer.hpp>

#include <plf/ast/Node.hpp>
#include <plf/ast/Attribute.hpp>
#include <plf/ast/Visitor.hpp>

#include <plf/ast/SymbolTable.hpp>

namespace plf {

//Forward Declarations
class FunctionDecl;

	
/**
* Flags for declarations
*/
enum class DeclFlags : unsigned char
{
	None		= 0,
	Public 		= 1<<0,
	Private  	= 1<<1,
	Protected   = 1<<2,
	
	Final		= 1<<3,
	Abstract	= 1<<4,
	Static 		= 1<<5,
	Const 		= 1<<6,
	
	External    = 1<<7
};

DeclFlags operator|(DeclFlags a, DeclFlags b);

DeclFlags operator&(DeclFlags a, DeclFlags b);

	
/**
* Basic class for declarations
*/
class Declaration : public Node
{
public:
	Declaration(const NodeKind kind) : Node(kind) {}
	DeclFlags flags;
	
	/// Is a type declaration or function
	virtual bool isType() const { return false; }
	/// is a instance declaration
	virtual bool isInstance() const { return false; }
	
	
	//TODO quick cast required?
	operator DeclPtr () { return to<Declaration>(); }
};


/**
* A package or module
* represents a object file (*.o) 
*/
class ModDecl final : public Declaration
{
public:
	static const NodeKind Kind = NodeKind::ModDecl;
	ModDecl() : Declaration(Kind) {}
	inline NodePtr accept(Visitor& v, ParamPtr& arg) final { return v.visit(*this, arg); }
	
	AttrList attribs;
	DeclList decls;	
	BufferPtr name;
	
	//all lists are private add "add","remove" functions?
};

/**
* Import Decl
* import a = a.b.c;
* import a.b.c;
*/
class UseDecl final : public Declaration
{
public:
	static const NodeKind Kind = NodeKind::UseDecl;
	UseDecl() : Declaration(Kind) {}
	inline NodePtr accept(Visitor& v, ParamPtr& arg) final { return v.visit(*this, arg); }
	
	BufferPtr ident;
	List<BufferPtr> importPath;
};

/**
* All declaration which defines new types
*/
class TypeDecl : public Declaration
{
public:
	TypeDecl(const NodeKind kind) : Declaration(kind) {}
	
	//overloads?
	//forwardref
	
	BufferPtr ident;

	//attributes
	virtual inline bool isType() const final { return true; }
	virtual inline bool isInstance() const final { return false; }
};

/** 
* Class Declaration
*/
class ClassDecl final : public TypeDecl
{
public:
	static const NodeKind Kind = NodeKind::ClassDecl;
	ClassDecl() : TypeDecl(NodeKind::ClassDecl) {}
	NodePtr accept(Visitor& v, ParamPtr& arg) final { return v.visit(*this, arg); }
	
	BufferPtr name;
	
	//ctors
	PtrList<FunctionDecl> ctors;
	SharedPtr<FunctionDecl> dtor;
	PtrList<ClassDecl> inherits;
	
	//tpl parameter (can be types or instances)

	//nested
	//template
	DeclList decls;
	//InstanceList List<ClassDecl>
};

/**
* Trait Declaration
*/
class TraitDecl final : public TypeDecl
{
public:
	static const NodeKind Kind = NodeKind::TraitDecl;
	TraitDecl() : TypeDecl(NodeKind::TraitDecl) {}
	inline NodePtr accept(Visitor& v, ParamPtr& arg) final { return v.visit(*this, arg); }
	
};

/**
* Struct Declaration
*/
class StructDecl : public TypeDecl
{
public:
	static const NodeKind Kind = NodeKind::StructDecl;
	StructDecl() : TypeDecl(NodeKind::StructDecl) {}
	inline NodePtr accept(Visitor& v, ParamPtr& arg) final { return v.visit(*this, arg); }

};

/**
* Enum Declaration
*/
class EnumDecl : public TypeDecl
{
public:
	static const NodeKind Kind = NodeKind::EnumDecl;
	EnumDecl() : TypeDecl(NodeKind::EnumDecl) {}
	inline NodePtr accept(Visitor& v, ParamPtr& arg) final { return v.visit(*this, arg); }

	//how?
	//name -> expr?
};

/**
* Alias Declaration
*/
class AliasDecl : public TypeDecl
{
public:
	BufferPtr ident;
	TypePtr type;
};

/**
* Function parameter
*/
struct FunctionParameter
{
	bool readonly;		//can not changed
	BufferPtr ident;	//name
	TypePtr type;		//datatype
	
	FunctionParameter(bool ro, const BufferPtr& id, const TypePtr& type)
		: readonly(ro), ident(id), type(type)
	{
	}
};

/**
* Function Declaration
*/
class FunctionDecl final : public Declaration
{
public:
	static const NodeKind Kind = NodeKind::FunctionDecl;
	FunctionDecl() : Declaration(NodeKind::FunctionDecl) {}
	inline NodePtr accept(Visitor& v, ParamPtr& arg) final { return v.visit(*this, arg); }

	BufferPtr name;
	List<FunctionParameter> params;
	StmtPtr body;
	TypePtr returnType; //leave unset for unkown?
	
	bool classFunc = false; //means parent is classdecl
	bool tplFunc = false;	//means is template function
	bool extFunc = false;	//extension function
	
	//ctf - compile time function
	
	bool hasBody() const { return body ? true : false; }
	//template 
	//nested 
	
	//functions overloads?
	//List<FunctionDecl> overloads
	//List<FunctionDecl> instances;
};

/**
* Instance Type
*/
enum class InstanceType
{
	Variable,
	Value,
	Constant
};

/**
* All declarations which creates an instance of a type
*/
class InstanceDecl : public Declaration
{
public:
	static const NodeKind Kind = NodeKind::InstanceDecl;
	InstanceDecl() : Declaration(Kind) {}
	inline NodePtr accept(Visitor& v, ParamPtr& arg) final { return v.visit(*this, arg); }
	
	//TODO only 3 types? var/let/const
	
	virtual inline bool isType() const final { return false; }
	virtual inline bool isInstance() const final { return true; }
	
	InstanceType itype;
	
	BufferPtr name;
	TypePtr type;
	ExprPtr init;
};


} //end namespace plf

#endif //__PLF_DECLARATION_HPP__


