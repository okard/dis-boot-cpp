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
#ifndef __PLF_TYPES_HPP__
#define __PLF_TYPES_HPP__

#include <plf/base/Buffer.hpp>
#include <plf/ast/Node.hpp>

namespace plf {

//TODO necessary to inherit from Node?

/**
* Represents a Datatype
*/
class Type : public Node
{
public:
	Type(const NodeKind kind) : Node(kind) {}
	
};

/**
* Unkown Type
*/
class UnkownType : public Type
{
private:
	UnkownType() : Type(NodeKind::UnkownType) {}
public:
	UnkownType& getInstance(); //get sharedptr
};

/**
* Primary Number Types
*/
class PrimaryType : public Type
{
public:
	const size_t size;
	const char* name;
	const bool signedT;
	
	//floating point?
	
	PrimaryType(size_t size, const char* name, bool signedT);
};

extern PrimaryType typeInt32;
extern PrimaryType typeUInt32;


//cpp: PrimaryType typeInt32(4, "Int32");

class DeclType : public Type
{
public:
	DeclType() : Type(NodeKind::DeclType) {}

	DeclPtr decl;
	//symbol& sym?
	
	//bool ref/ptr
};

/**
* Parsed Type to has solved
* results in DeclType
*/
class UnsolvedType : public Type
{
public:
	UnsolvedType() : Type(NodeKind::UnsolvedType) {}
	
	List<BufferPtr> idents;
};


//array types
//path type
//constraint type


} //end namespace plf

#endif // __PLF_TYPES_HPP__
