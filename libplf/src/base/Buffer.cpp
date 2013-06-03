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
#include <plf/base/Buffer.hpp>

#include <cstring>

using namespace plf;

Buffer::Buffer()
	: size_(0), buffer_(nullptr)
{
}

Buffer::Buffer(size_t size)
	: size_(size), buffer_(new byte[size])
{
}

Buffer::Buffer(const byte* ptr)
	: size_(strlen(ptr)), buffer_(new byte[strlen(ptr)])
{
	insert(ptr, size_);
}

Buffer::Buffer(const byte* ptr, size_t size)
	: size_(size), buffer_(new byte[size])
{
	insert(ptr, size);
}

Buffer::Buffer(const Buffer& buf)
	: size_(buf.size_), buffer_(new byte[buf.size_])
{
	insert(buf.buffer_, size_);
}

Buffer::~Buffer()
{
	if(buffer_)
		delete[] buffer_;
}

void Buffer::alloc(size_t size)
{
	//resize?
	if(size_ >= size)
		return;
	
	//only one allocation?
	if(buffer_)
		delete[] buffer_;
	
	buffer_ = new byte[size];
	size_ = size;
}

size_t Buffer::insert(const byte* ptr, size_t size)
{
	//TODO wrap?
	if(!buffer_)
		return 0;
	
	size = size > size_ ? size_ : size;
	memcpy(buffer_, ptr, size);
	return size;
}

Buffer& Buffer::operator=(const Buffer& buf)
{
	if(this != &buf)
	{
		alloc(buf.size_);
		insert(buf.buffer_, buf.size_);
	}
	return *this;
}

bool Buffer::operator==(Buffer const& buf)
{
	if(this == &buf)
		return true;
	
	return memcmp(buffer_, buf.buffer_, size_);
}
	
bool Buffer::operator==(const char* str)
{
	return memcmp(buffer_, str, size_);
}



