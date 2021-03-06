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
#ifndef PLF_SOURCE_HPP
#define PLF_SOURCE_HPP

#include <cstddef>
#include <memory>

namespace plf {
	
//Forward Declaration:	
class Buffer;
	
//Source ID
typedef unsigned int SourceId;

enum class Encoding 
{
	ASCII,
	UTF8,
	UTF16,
	UTF32
};

/**
* Source representation
* can be a file, a memory string ...
*/
class Source
{
private:
	SourceId id_;
	
protected:
	Encoding encoding_;

public:	
	
	//get buffer?

	//reads a specific chunk of source into buffer
	//virtual size_t readComplete(Buffer&, size_t start, size_t end) = 0;
	
	//read to buffer
	virtual size_t readComplete(Buffer&) = 0;

	/**
	* Get id for source manager
	*/
	inline SourceId getId() const { return id_; }
	
	/**
	* Get encoding of source
	*/
	inline Encoding getEncoding() const { return encoding_; }
	
	/**
	* get an identifier for the source file
	*/
	virtual const char* identifier() const;
	
	/**
	* get source size
	*/
	virtual size_t size() const = 0;
	
	
	friend class SourceManager;
};


typedef std::shared_ptr<Source> SourcePtr;
	
	
} //end namespace

#endif //PLF_SOURCE_HPP
