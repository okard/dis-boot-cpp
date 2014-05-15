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
#ifndef PLF_TYPES_HPP
#define PLF_TYPES_HPP

#include <plf/base/Buffer.hpp>
#include <plf/ast/Node.hpp>

namespace plf {

/**
 * @brief The TypeKind enum
 * type kinds
 */
enum class TypeKind
{
	UnkownType, //no type information
	PrimaryType,
	DeclType,
	UnsolvedType, //unsolved type
	OwnedPtrType,
	BorrowedPtrType,
	RawPtrType,
	ArrayType,
	FunctionType,

	CTType		//compile time type
};


//TODO necessary to inherit from Node?

/**
* Represents a Datatype
*/
class Type
{
public:

	const TypeKind kind;

	Type(const TypeKind kind) : kind(kind) {}

	//bool validType = false;

	//compare operator?
	//implicit casting?


	/**
	* Create helper
	*/
	template<class T, typename... Args>
	static SharedPtr<T> create(Args... args)
	{
		TypeKind k = T::Kind; //make shure each node has it?
		k=k; //avoid warning
		return std::make_shared<T>(args...);
	}
};

/**
* Unkown Type
*/
class UnkownType : public Type
{
public:
	static const TypeKind Kind = TypeKind::UnkownType;
private:
	UnkownType() : Type(Kind) {}
public:


	static TypePtr getInstance(); //get sharedptr
};

/**
* Primary Number Types
*/
class PrimaryType : public Type
{
public:
	static const TypeKind Kind = TypeKind::PrimaryType;
	const size_t size;
	const char* name;
	const bool signedT;

	
	//floating point?
	
	PrimaryType(size_t size, const char* name, bool signedT);


	//static const TypePtr
	static TypePtr TypeI8();
	static TypePtr TypeU8();
	static TypePtr TypeI16();
	static TypePtr TypeU16();
	static TypePtr TypeI32();
	static TypePtr TypeU32();
	static TypePtr TypeI64();
	static TypePtr TypeU64();
	static TypePtr TypeF32();
	static TypePtr TypeF64();

};



//cpp: PrimaryType typeInt32(4, "Int32");

class DeclType : public Type
{
public:
	static const TypeKind Kind = TypeKind::DeclType;
	DeclType() : Type(Kind) {}

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
	static const TypeKind Kind = TypeKind::UnsolvedType;
	UnsolvedType() : Type(Kind)
	{}
	
	List<BufferPtr> idents;
};

/**
* Owned Pointer Type
*/
class OwnedPtrType : public Type
{
public:
	static const TypeKind Kind = TypeKind::OwnedPtrType;
	OwnedPtrType() : Type(Kind) {}
	TypePtr targetType;
};

/**
* Owned Pointer Type
*/
class BorrowedPtrType : public Type
{
public:
	static const TypeKind Kind = TypeKind::BorrowedPtrType;
	BorrowedPtrType() : Type(Kind) {}
	TypePtr targetType;
};

/**
* Owned Pointer Type
*/
class RawPtrType : public Type
{
public:
	static const TypeKind Kind = TypeKind::RawPtrType;
	RawPtrType() : Type(Kind) {}
	TypePtr targetType;
};

/**
* @brief The ArrayType class
*/
class ArrayType : public Type
{
public:
	static const TypeKind Kind = TypeKind::ArrayType;
	ArrayType() : Type(Kind)
	{}

	TypePtr targetType;

	int size = 0;

};


/**
* @brief The ArrayType class
*/
class FunctionType : public Type
{
public:
	static const TypeKind Kind = TypeKind::FunctionType;
	FunctionType() : Type(Kind)
	{}

	//TODO vararg
	List<TypePtr> params;
	TypePtr returnType;
};

/**
 * @brief A CompileTime Type
 */
class CTType : public Type
{
public:
	static const TypeKind Kind = TypeKind::CTType;
	CTType() : Type(Kind)
	{}

	TypePtr type;
};


//seperate path type?
//constraint type

} //end namespace plf

#endif // PLF_TYPES_HPP
