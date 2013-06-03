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

#include <plf/ast/Node.hpp>
#include <plf/ast/Visitor.hpp>

namespace plf {
	
/**
* Flags for declarations
*/
enum class DeclFlags : unsigned char
{
	Public 		= 1<<0,
	Private  	= 1<<1,
	Protected   = 1<<2,
	Final		= 1<<3,
	Abstract	= 1<<4,
	Static 		= 1<<5,
	Const 		= 1<<6
};
	
/**
* Basic class for declarations
*/
class Declaration : public Node
{

private:

public:
	DeclFlags flags;
	
	//isTypeDecl
	//isInstanceDecl


	//operator DeclPtr () { return std::static_pointer_cast<Declaration>(shared_from_this()); }
};

/**
* All declaration which defines new types
*/
class TypeDecl : public Declaration
{
};

/**
* A package
*/
class PackageDecl final : public TypeDecl
{
public:
	static const NodeKind Kind = NodeKind::PackageDecl;

	DeclList decls;
	
	//symbol table mangled names
	
	NodePtr accept(Visitor& v, ParamPtr& arg) final { return v.visit(*this, arg); }
};

//import decl?

/** 
* Class Declaration
*/
class ClassDecl final : public Declaration
{
public:
	static const NodeKind Kind = NodeKind::ClassDecl;

	DeclList decls;
	//InstanceList
	
	
	NodePtr accept(Visitor& v, ParamPtr& arg) final { return v.visit(*this, arg); }
};

//trait

//function

//struct

//enum

//type

//alias

/**
* All declarations which creates an instance of a type
*/
class InstanceDecl : public Declaration
{
};

/**
* 
*/
class VariableDecl : public InstanceDecl
{
public:
	static const NodeKind Kind = NodeKind::VariableDecl;
	
	//id BufferPtr
	TypePtr type;
	ExprPtr init;
};


//val
//const



} //end namespace plf

#endif //__PLF_DECLARATION_HPP__


