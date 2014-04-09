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

#include <plf/base/Exception.hpp>

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
	//TODO check
	memcpy(buffer_, ptr, size_);
}

Buffer::Buffer(const byte* ptr, size_t size)
	: size_(size), buffer_(new byte[size])
{
	memcpy(buffer_, ptr, size_);
}

Buffer::Buffer(const Buffer& buf)
	: size_(buf.size_), buffer_(new byte[buf.size_])
{
	memcpy(buffer_, buf.buffer_, size_);
}

Buffer::~Buffer()
{
	if(buffer_)
	{
		delete[] buffer_;
		buffer_ = nullptr;
		size_ = 0;
	}
}



Buffer& Buffer::operator=(const Buffer& buf)
{
	if(this != &buf && buf.size() > 0)
	{
		alloc(buf.size_);
		memcpy(buffer_, buf.buffer_, size_);
	}
	return *this;
}

Buffer& Buffer::operator=(const char* str)
{
	auto size = strlen(str);
	
	if(size > 0)
	{
		alloc(size);
		memcpy(buffer_, str, size_);
	}
	
	return *this;
}

bool Buffer::operator==(Buffer const& buf) const
{
	if(this == &buf)
		return true;
		
	//compare with the bigger one?
	auto size = buf.size() < size_ ? size_ : buf.size();
	return memcmp(buffer_, buf.buffer_, size)==0;
}
	
bool Buffer::operator==(const char* str) const
{
	//compare with the bigger one?
	auto size = strlen(str);
	size = size < size_ ? size_ : size;
	return memcmp(buffer_, str, size)==0;
}

void Buffer::alloc(size_t size)
{
	free();
	
	//no allocation required
	if(size == 0)
		return;
	
	//set new buffer
	buffer_ = new byte[size];
	size_ = size;
	memset (buffer_,'\0',size);
}

void Buffer::resize(size_t size)
{
	//new buffer, copy old content
	auto new_buf = new byte[size];
	memset (new_buf,'\0',size);
	memcpy(new_buf, buffer_, size < size_ ? size : size_);
	free(); //free old buffer

	//set actual buffer and size
	buffer_ = new_buf;
	size_ = size;
}

void Buffer::free()
{
	//delete old buffer
	if(buffer_)
	{
		delete[] buffer_;
		buffer_ = nullptr;
		size_ = 0;
	}
}

//debug
#include<iostream>
void Buffer::dump()
{
	std::cout << "Dump Buffer: " << std::endl;
	std::cout.write(buffer_, size_);
	std::cout << std::endl;
}




//void * memset ( void * ptr, int value, size_t num );
// memset(buffer_, '\0', size_);

