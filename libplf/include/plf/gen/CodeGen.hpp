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
#ifndef __PLF_CODEGEN_HPP__
#define __PLF_CODEGEN_HPP__

#include <plf/gen/Target.hpp>

namespace plf {
	
class PackageDecl;
	
/**
* Object File
*/
struct ObjectFile
{
	//filename buffer filename_
	//stream?
};

/**
* Code Generator Interface
*/
class CodeGen
{
public:
	//DAST -> TargetFile
	
	//use objectfile interface generate in memory obejct files and so on
	virtual void gen(const PackageDecl&, const Target& t, ObjectFile& file) = 0;
	
	//ObjectFile gen(PackageDecl n, Param p);
	//ObjectFile gen(NodePtr n, Parameter);
};

} //end namespace plf

#endif //__PLF_CODEGEN_HPP__
