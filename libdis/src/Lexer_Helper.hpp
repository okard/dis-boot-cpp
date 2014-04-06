/*
Dis Programming Language Frontend Library

Copyright (c) 2014 okard

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
#ifndef DIS_LEXER_HELPER_HPP
#define DIS_LEXER_HELPER_HPP

namespace {

//todo generic utf32 int as char

/// Is a whitespace character
inline static bool isWhitespace(char c)
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

///is c a alpha character
inline static bool isAlpha(char c)
{
	return (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| c == '_';
}

//is c a numeric character
inline static bool isNumeric(char c)
{
	return (c >= '0' && c <= '9');
}

///is c a binary character
inline bool isBin(char c)
{
	return(c == '0' || c == '1');
}

///is c a hex character
inline static bool isHex(char c)
{
	return (c >= 'a' && c <= 'f')
		|| (c >= 'A' && c <= 'F')
		|| (c >= '0' && c <= '9');
}

//readable ascii: 0x21-0x7E


}

#endif //DIS_LEXER_HELPER_HPP
