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
#include <plf/base/Source.hpp>
#include <plf/base/SourceReader.hpp>

#include "Token.hpp"

namespace dis {

/**
* Dis Token Lexer
*/
class Lexer
{
private:
	//reader to handle source input
	plf::SourceReader reader_;
	
	//lex doc comments
	bool doc_comment_enabled;

	size_t column_ = 0;
	size_t line_ = 0;

public:
	Lexer();
	~Lexer();
	
	void open(plf::SourcePtr srcptr);
	
	void next(Token& token);
	
	// Loc/ OffsetLoc stuff

	//TODO Lexer hooks for inline asm
		//Stackable ILexer Interfaces Stack<ILexer>
		//push(lexer), pop(lexer)

	//get SavePoint& 
	//load SavePoint&

	
private:
	void lexToken(Token& tok);
	void lexId(Token& tok);
	void lexString(Token& tok);
	void lexNumber(Token& tok);
	void lexComment(Token& tok);
	
	//use int32 as utf32 representation?
	inline void nextChar();
	inline char& current();
	inline char& peekChar(size_t n);
	inline void skip(int i);
	
	inline void checkForChar(Token& tok, char c, TokenId id);
};


} //end namespace dis

#endif // __DIS_LEXER_HPP__

