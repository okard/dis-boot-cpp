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
#include <plf/ast/Type.hpp>

#include <plf/base/Exception.hpp>

using namespace plf;


PrimaryType::PrimaryType(size_t size, const char* name, bool signedT)
	: Type(Kind), size(size), name(name), signedT(signedT)
{
}

TypePtr PrimaryType::TypeI8()
{
	static TypePtr instance(new PrimaryType(1, "i8", true));
	return instance;
}

TypePtr PrimaryType::TypeU8()
{
	static TypePtr instance(new PrimaryType(1, "u8", false));
	return instance;
}

TypePtr PrimaryType::TypeI16()
{
	static TypePtr instance(new PrimaryType(2, "i16", true));
	return instance;
}

TypePtr PrimaryType::TypeU16()
{
	static TypePtr instance(new PrimaryType(2, "u16", false));
	return instance;
}

TypePtr PrimaryType::TypeI32()
{
	static TypePtr instance(new PrimaryType(4, "i32", true));
	return instance;
}

TypePtr PrimaryType::TypeU32()
{
	static TypePtr instance(new PrimaryType(4, "u32", false));
	return instance;
}

TypePtr PrimaryType::TypeI64()
{
	static TypePtr instance(new PrimaryType(8, "i64", true));
	return instance;
}

TypePtr PrimaryType::TypeU64()
{
	static TypePtr instance(new PrimaryType(8, "u64", false));
	return instance;
}

TypePtr PrimaryType::TypeF32()
{
	throw Exception("Not implemented");
}

TypePtr PrimaryType::TypeF64()
{
	throw Exception("Not implemented");
}

/*
PrimaryType typeInt16(2, "i16", true);
PrimaryType typeUInt16(2, "u16", false);
PrimaryType typeInt32(4, "i32", true);
PrimaryType typeUInt32(4, "u32", false);
PrimaryType typeInt64(8, "i64", true);
PrimaryType typeUInt64(8, "u64", false);
*/


TypePtr UnkownType::getInstance()
{
	static TypePtr instance(new UnkownType());
	return instance;
}

