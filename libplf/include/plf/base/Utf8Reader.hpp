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
#ifndef PLF_UTF8READER_HPP
#define PLF_UTF8READER_HPP

#include <plf/base/Source.hpp>
#include <plf/base/Buffer.hpp>

namespace plf {

typedef unsigned int uchar; //codepoint

static_assert(sizeof(uchar) == 4, "unicode char requires size of 32 bits");

//todo rename it to UtfReader and add multiple reading modes?

/**
* @brief The Utf8Reader class
*/
class Utf8Reader
{
private:

	//ringbuffer reading?
	Buffer buf_;
	size_t currentPos_ = 0;
	size_t lastPos_ = 0;
public:

	//access to buffer to load from SourcePtr

	//create new buffer from slice(start, stop);

	uchar read_uchar();

	void reset();

	inline bool is_eob() const { return bytes_left()==0; }
	size_t bytes_left() const;
	inline size_t pos() const { return currentPos_; }
	inline size_t last_pos() const { return lastPos_; }
	inline const Buffer& buf() const { return buf_; }
	inline Buffer& buf() { return buf_; }
};


} //end namespace plf

#endif // PLF_UTF8READER_HPP
