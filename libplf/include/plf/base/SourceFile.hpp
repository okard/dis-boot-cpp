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
#ifndef PLF_SOURCEFILE_HPP
#define PLF_SOURCEFILE_HPP

#include <fstream>

#include "Source.hpp"

namespace plf {
	
	
class SourceFile : public Source
{
private:
	std::ifstream filestream_;
	size_t size_;
	
	const char* filename_;
	
	//Buffer filename_;
	//string filename
public:

	///Open file 
	void open(const char* filename);
	
	///read
	size_t readComplete(Buffer&);
	
	///return file name
	virtual const char* identifier() const;
	
	///return file size
	size_t size() const { return size_; }
};
	
} // end namespace plf

#endif // PLF_SOURCEFILE_HPP
