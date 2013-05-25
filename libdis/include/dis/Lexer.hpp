/*
Dis Programming Language Frontend Library

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
#ifndef __DIS_LEXER_HPP__
#define __DIS_LEXER_HPP__

#include <deque>

#include <plf/base/External.hpp>
#include <plf/base/Buffer.hpp>
#include <plf/base/Source.hpp>

#include "Token.hpp"

namespace dis {

/**
* Dis Token Lexer
*/
class Lexer
{
private:
	plf::Buffer buf_;
	plf::BufferView bufv_;
	plf::SourcePtr src_;
	
	//ring buffer?
	std::deque<Token> toklist_;
	
	//bool doc_comment_enabled;

public:
	Lexer();
	~Lexer();
	
	void open(plf::SourcePtr srcptr);
	
	Token& next();
	Token& peek(int num);
	
private:
	void pushToken();
	void lexId();
	void lexString();
	void lexNumber();
	void lexComment();
};


} //end namespace dis

#endif // __DIS_LEXER_HPP__

