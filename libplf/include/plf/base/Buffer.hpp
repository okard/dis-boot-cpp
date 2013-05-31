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
#ifndef __PLF_BUFFER_HPP__
#define __PLF_BUFFER_HPP__

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
	byte* buffer_;
	
public:

	Buffer();
	Buffer(size_t size);
	Buffer(const byte* ptr);
	virtual ~Buffer();
	
	void alloc(size_t size);
	
	size_t insert(const byte* ptr, size_t size);
	
	inline byte* operator[] (size_t offset) { return &buffer_[offset]; }
	
	inline size_t size() const { return size_; }
	inline byte* ptr() { return buffer_; }
	inline const byte* ptr() const { return buffer_; }
	
	
	
};

typedef SharedPtr<Buffer> BufferPtr;

	
} // end namespace plf

#endif // __PLF_BUFFER_HPP__
