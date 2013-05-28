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
#include <plf/base/SourceFile.hpp>

#include <plf/base/BufferView.hpp>

using namespace plf;


void SourceFile::open(const char* filename)
{
	size_ = 0;
	filestream_.open(filename, std::ifstream::in);
	
	if(filestream_) 
	{
		// get length of file:
		filestream_.seekg (0, filestream_.end);
		size_ = filestream_.tellg();
		filestream_.seekg (0, filestream_.beg);
	}
}

size_t SourceFile::read(BufferView& buf, size_t size)
{
	
	if( (buf.buffer().size() - buf.pos()) < size)
		size = buf.buffer().size() - buf.pos();
	//check for size
	//try to read
	//set limit
	
	if(filestream_)
	{
		filestream_.read(buf.ptr(), size);
		auto bytesRead = filestream_.gcount();
		buf.setLimit(bytesRead+buf.pos());
		return bytesRead;
	}
	
	return 0;
}
