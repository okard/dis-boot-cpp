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
#include <dis/Lexer.hpp>

using namespace dis;

////////////////////////
// Helper

inline bool isWhitespace(char c)
{
	switch(c)
	{
		case '\n':
		case '\r':
		case '\t':
		case ' ':
			return true;
		default:
			return false;
	}
}

inline bool isAlpha(char c) 
{
	return (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| c == '_';
}


Lexer::Lexer()
	: bufv_(buf_)
{
}

Lexer::~Lexer()
{
}

void Lexer::open(plf::SourcePtr srcptr)
{
	//read in the complete file
	auto size = srcptr->size();
	buf_.alloc(size);
	srcptr->read(bufv_, size);
	
	//TODO reset internal status
	
}


Token& Lexer::next()
{
	if(toklist_.empty())
	{
		pushToken();
		return toklist_.front();
	}
	
	//pop front the old one
	toklist_.pop_front();
	
	//return front
	return toklist_.front();
}

Token& Lexer::peek(int num)
{
	//check toklist_ count
	//push token until num in toklist
	//return element
}


void  Lexer::pushToken()
{
	toklist_.push_back(Token());
	Token& tok =  toklist_.back();
	
	char c;
	while(isWhitespace(c = bufv_.readChar()));
	
	if(bufv_.eob())
	{
		tok.id = TokenId::Eof;
		return;
	}
	
	//if is alpha lexId();
	//startpos = bufv_.pos()-1;
	
	switch(c)
	{
		case '{': 
			tok.id = TokenId::COBracket;
			break;
		case '}':
			tok.id = TokenId::CCBracket;
			break;
		
		
		default: 
			//error unkown char
			throw "err";
	}
}



void Lexer::lexId()
{
}

void Lexer::lexString()
{
}

void Lexer::lexNumber()
{
}

void Lexer::lexComment()
{
}
