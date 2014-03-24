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
#ifndef PLF_BUFFER_HPP
#define PLF_BUFFER_HPP

#include <cstddef>
#include <plf/base/External.hpp>

namespace plf {

/**
* Simple byte buffer
*/
class Buffer
{
public:
	typedef char byte;
private:
	size_t size_;
	byte* buffer_; //TODO std::unique_ptr???
	
public:

	Buffer();
	Buffer(size_t size);
	Buffer(const byte* ptr);
	Buffer(const byte* ptr, size_t size);
	Buffer(const Buffer& buf);
	virtual ~Buffer();
	
	//TODO set to 0
	//clear();
	//empty();

	//allocates the buffer with a new size, old buffer data is freed
	void alloc(size_t size);

	//realloc
	//append

	//assign other buffer/ 0-terminated strings
	Buffer& operator=(const Buffer& buf);
	Buffer& operator=(const char* str);
	
	//compare
	bool operator==(const Buffer& buf) const;
	bool operator==(const char* str) const;
	
	//size of buffer memory
	inline size_t size() const { return size_; }

	//access to buffer memory
	inline const byte* ptr() const { return buffer_; }
	inline byte* ptr() { return buffer_; }
	
	//debug
	void dump();
};

typedef SharedPtr<Buffer> BufferPtr;

	
} // end namespace plf

#endif //PLF_BUFFER_HPP
