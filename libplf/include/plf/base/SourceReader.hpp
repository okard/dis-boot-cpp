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
#ifndef PLF_SOURCEREADER_HPP
#define PLF_SOURCEREADER_HPP

#include <plf/base/Source.hpp>
#include <plf/base/Buffer.hpp>

namespace plf {

/**
* Helper class for reading from source objects
*/
class SourceReader
{
private:
	Buffer buf_;
	SourcePtr source_;
	
	size_t startPos_ = 0;
	size_t endPos_ = 0;
	size_t currentPos_ = 0;
	
public:
	
	//load a source file
	void load(SourcePtr& ptr);
	
	//copy a specific part of source to buffer
	void copyto(Buffer& buf, size_t start, size_t size);
	
	//skip an amount of bytes
	void skip(size_t size)
	{
		//TODO check limits
		currentPos_ += size;
	}
	
	//peek a value at offset relative to current position
	template<typename T>
	inline T& peek(size_t offset) 
	{ 
		//check position
		return *reinterpret_cast<T*>(&buf_.ptr()[currentPos_+offset]); 
	}

	//skip a value
	template<typename T>
	inline void next() 
	{ 
		//check space
		//byte addressed
		currentPos_ += sizeof(T);
	}
	
	//return value at current position
	template<typename T>
	inline T& current() 
	{ 
		return *reinterpret_cast<T*>(&buf_.ptr()[currentPos_]); 
	}
	
	
	//return current position
	inline size_t pos() const { return currentPos_; }
	//status end of source
	inline bool eos() const { return buf_.is_empty() || (currentPos_ >= endPos_); }
	
private:
	inline size_t leftSpace() const { return  endPos_ - currentPos_; }

};
	
} //end namespace plf


#endif //PLF_SOURCEREADER_HPP
