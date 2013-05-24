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

namespace plf {
	

//TODO Create a more generic interface for MMAP IO?

/**
* Buffer
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
	virtual ~Buffer();
	
	void alloc(size_t size);
	
	inline byte* operator[] (size_t offset) { return &buffer_[offset]; }
	
	inline size_t size() const { return size_; }
	inline byte* ptr() { return buffer_; }
	inline const byte* ptr() const { return buffer_; }
	
};

/**
* A view onto buffer
*/
class BufferView
{
private:
	Buffer& buffer_;
	size_t pos_;
	size_t limit_;
public:
	
	/// A Buffer View
	BufferView(Buffer& buf);
	/// Destructor
	virtual ~BufferView();
	
	///Deliver char
	inline unsigned char readChar() { return read<unsigned char>(); }
	
	//short for utf16 char
	//int for utf32 char
	
	template<typename T>
	inline T read() { T* t = reinterpret_cast<T*>(buffer_[pos_]); pos_ += sizeof(T); return *t; }
	
	template<typename T>
	inline T peek(size_t offset) { return *reinterpret_cast<T*>(buffer_[pos_+offset]); }
	
	inline unsigned char peekChar(size_t offset) { return peek<unsigned char>(offset); }
	
	/// set position
	inline void set(size_t p) { pos_ = p <= 0 ? 0 : p >= limit_ ? limit_ : p;  }
	
	inline void setLimit(size_t l) { /*TODO check*/ limit_ = l; }
	
	/// is position at the end of buffer
	inline bool eob() const { return pos_>= limit_; }
	
	/// access to current positon in buffer as ptr
	inline Buffer::byte* ptr() { return buffer_[pos_]; }
	/// return buffer
	inline const Buffer& buffer() const { return buffer_; }
	/// return buffer
	inline Buffer& buffer() { return buffer_; }
	/// return position
	inline size_t pos() { return pos_; }
};
	
} // end namespace plf

#endif // __PLF_BUFFER_HPP__
