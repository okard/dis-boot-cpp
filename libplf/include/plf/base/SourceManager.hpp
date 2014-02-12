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
#ifndef __PLF_SOURCEMANAGER_HPP__
#define __PLF_SOURCEMANAGER_HPP__

#include <plf/base/Source.hpp>

#include <memory>
#include <vector>

namespace plf {

/**
* Managing sources
*/
class SourceManager
{
private:
	SourceManager() = default;
	SourceManager(const SourceManager& sm) = delete;
	virtual ~SourceManager() = default;
	
	SourceManager& operator=(const SourceManager&) = delete;
	
	/// source storage
	//index == SourceId
	std::vector<SourcePtr> sources_;
	
public:

	///get SourcePtr for id
	SourcePtr get(SourceId id);
	
	///load a physical source file
	SourcePtr loadFile(const char*);
	
	//SourcePtr loadBuffer(BufferPtr& buf);
	//SourcePtr loadBuffer(Buffer& buf);
	
	
	///get singleton instance of source manager
	static SourceManager& getInstance();
};	

	
} //end namespace plf


#endif
