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

#include <cstring>
#include <cassert>
//#include <unordered_map>

#include <plf/base/FormatException.hpp>

#include <iostream>

using namespace dis;

////////////////////////
// Helper

/// Is a whitespace character
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

///is c a alpha character
inline bool isAlpha(char c) 
{
	return (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| c == '_';
}

//is c a numeric character
inline bool isNumeric(char c)
{
	return (c >= '0' && c <= '9');
}

///is c a binary character
inline bool isBin(char c)
{
	return(c == '0' || c == '1');
}

///is c a hex character
inline bool isHex(char c)
{
	return (c >= 'a' && c <= 'f')
		|| (c >= 'A' && c <= 'F')
		|| (c >= '0' && c <= '9');
}

//readable ascii: 0x21-0x7E

inline void checkKeyword(Token& tok);

Lexer::Lexer()
{
}

Lexer::~Lexer()
{
}

void Lexer::open(plf::SourcePtr srcptr)
{
	//read in the complete file
	/*auto size = srcptr->size();
	buf_ = plf::Buffer(size);
	srcptr->readComplete(buf_);
	*/
	
	reader_.load(srcptr);
	
	//TODO reset internal status
	toklist_.clear();
	
	column_ = 1;
	line_ = 1;
}


Token& Lexer::next()
{
	//pop front the old one
	if(!toklist_.empty())
		toklist_.pop_front();
	
	//if now empty add a new one
	if(toklist_.empty())
	{
		toklist_.push_back(Token());
		Token& tok =  toklist_.back();
		lexToken(tok);
	}
	
	//return actual at front
	return toklist_.front();
}

Token& Lexer::peek(int num)
{
	//check toklist_.size() 
	//push token until num in toklist
	//return element
}


void  Lexer::lexToken(Token& tok)
{	
	if(reader_.eos())
	{
		tok.id = TokenId::Eof;
		tok.loc.column = column_;
		tok.loc.line = line_;
		return;
	}
	
	while(isWhitespace(current()))
		nextChar();
		
	char c = current();
	tok.loc.column = column_;
	tok.loc.line = line_;
	
	//id and keywords
	if(isAlpha(c))
	{
		lexId(tok);
		return;
	}
	
	//numbers
	if(isNumeric(c))
	{
		lexNumber(tok);
		return;
	}
	
	//strings
	if(c == '"')
	{
		lexString(tok);
		return;
	}
	
	switch(c)
	{
		case '{': tok.id = TokenId::COBracket; nextChar(); break;
		case '}': tok.id = TokenId::CCBracket; nextChar(); break;
		case '(': tok.id = TokenId::ROBracket; nextChar(); break;
		case ')': tok.id = TokenId::RCBracket; nextChar(); break;
		case '[': tok.id = TokenId::SOBracket; nextChar(); break;
		case ']': tok.id = TokenId::SCBracket; nextChar(); break;
		case '.': tok.id = TokenId::Dot; nextChar(); break;
		case ',': tok.id = TokenId::Comma; nextChar(); break;
		case ':': tok.id = TokenId::Colon; nextChar(); break;
		case ';': tok.id = TokenId::Semicolon; nextChar(); break;
		case '=': tok.id = TokenId::Assign; nextChar(); break;
		case '@': tok.id = TokenId::At; nextChar(); break;
		case '&': tok.id = TokenId::And; nextChar(); break;
		case '|': tok.id = TokenId::Or; nextChar(); break;
		case '!': tok.id = TokenId::EPoint; nextChar(); break;
		case '#': tok.id = TokenId::Sharp; nextChar(); break;
		case '~': tok.id = TokenId::Tilde; nextChar(); break;
		case '+': tok.id = TokenId::Plus; nextChar(); break;
		case '-': tok.id = TokenId::Minus; nextChar(); break;
		case '*': tok.id = TokenId::Mul; nextChar(); break;
		case '/': tok.id = TokenId::Div; nextChar(); break;
		case '%': tok.id = TokenId::Mod; nextChar(); break;
		case '<': tok.id = TokenId::Less; nextChar(); break;
		case '>': tok.id = TokenId::Greater; nextChar(); break;
		

		//TODO FIXIT
		case 0x00:
			//throw plf::FormatException("Nullchar");
			tok.id = TokenId::Eof;
			return;
		
		default: 
			//error unkown char
			throw plf::FormatException("(%d, %d) Unknown Character: %02x", line_, column_, c);
	}
	
	//check for comments
	if(tok.id == TokenId::Div && current() == '/')
	{
		while(!reader_.eos() && current() != '\n')
			nextChar();
		lexToken(tok);
		return;
	}
	
	//TODO /* */ comments
	
	//TODO /** */ and /// doc comments
	
	//check double token
	switch(tok.id)
	{
		case TokenId::DblColon: checkForChar(tok, ':', TokenId::DblColon); break;
		case TokenId::Dot: checkForChar(tok, '.', TokenId::DotDot); break;
		case TokenId::Sharp: checkForChar(tok, '!', TokenId::Shebang); 
							 checkForChar(tok, '[', TokenId::AttrStart);
							 break;
		case TokenId::EPoint: checkForChar(tok, '=', TokenId::NEqual); break;
		case TokenId::Assign: checkForChar(tok, '=', TokenId::Equal); break;
		case TokenId::Plus: checkForChar(tok, '=', TokenId::PlusAssign); 
							checkForChar(tok, '+', TokenId::PlusPlus);
							break;
		case TokenId::Minus: checkForChar(tok, '=', TokenId::MinusAssign); 
							 checkForChar(tok, '>', TokenId::Lambda);
							 checkForChar(tok, '-', TokenId::MinusMinus);
							 break;
		case TokenId::Mul: checkForChar(tok, '=', TokenId::MulAssign); break;
		case TokenId::Div: checkForChar(tok, '=', TokenId::DivAssign); break;
		case TokenId::Mod: checkForChar(tok, '=', TokenId::ModAssign); break;
		case TokenId::Tilde: checkForChar(tok, '=', TokenId::TildeAssign); break;
		case TokenId::Less: checkForChar(tok, '=', TokenId::LTE); break;
		case TokenId::Greater: checkForChar(tok, '=', TokenId::GTE); break;
		case TokenId::And: checkForChar(tok, '&', TokenId::LAnd); break;
		case TokenId::Or: checkForChar(tok, '|', TokenId::LOr); break;
		default:break;
	}
	
	//check triple token
	switch(tok.id)
	{
		case TokenId::DotDot: checkForChar(tok, '.', TokenId::TripleDot); break;
		default:break;
	}
}


void Lexer::lexId(Token& tok)
{
	tok.id = TokenId::Ident;
	
	int i;
	for(i = 0; isAlpha(peekChar(i)) || isNumeric(peekChar(i)); i++); //TODO check for eob
	
	//read bufv_.ptr() size i into token
	//std::string str(bufv_.ptr(), i);
	//std::cout << "Lex id: " << str << std::endl;
	
	//add the id to token buffer
	reader_.copyto(*tok.buffer, reader_.pos(), i); 
	
	checkKeyword(tok);
	
	//skip over
	reader_.skip(i);
	column_ += i;
}

void Lexer::lexNumber(Token& tok)
{
	tok.id = TokenId::IntLiteral;
	
	assert(isNumeric(current()));
	
	//binary
	if(current() == '0' 
	&& peekChar(1) == 'b')
	{
		tok.id = TokenId::BinaryLiteral;
		nextChar(); //now b
		nextChar(); //now first digit
		
		int i;
		for(i = 0; isBin(peekChar(i)); i++); //TODO check for eob
		
		reader_.copyto(*tok.buffer, reader_.pos(), i);
		//skip over
		reader_.skip(i);
		return;
	}
	
	//hex
	if(current() == '0' 
	&& peekChar(1) == 'x')
	{
		tok.id = TokenId::HexLiteral;
		nextChar(); //now x
		nextChar(); //now first digit
		
		int i;
		for(i = 0; isHex(peekChar(i)); i++); //TODO check for eob
		reader_.copyto(*tok.buffer, reader_.pos(), i);
		//skip over
		reader_.skip(i);
		return;
	}
	
	
	int i=0;
	for(i = 0; isNumeric(peekChar(i)); i++);
	
	//float/doubles
	if(peekChar(i) == '.')
	{
		tok.id = TokenId::FloatLiteral;
		i++;
		for(; isNumeric(peekChar(i)); i++);
	}
	
	reader_.copyto(*tok.buffer, reader_.pos(), i);
		//skip over
		
	
	//normal number
	//dot == double
	//only one dot allowed
	//if dot and ends with .f or .{num}f
	
	//skip over
	reader_.skip(i);
	column_ += i; 
}

void Lexer::lexString(Token& tok)
{
	assert(current() == '"');
	tok.id = TokenId::StringLiteral;
	
	nextChar(); //skip "
	int i;
	for(i = 0; peekChar(i) != '"'; i++); //TODO check for eob
	
	//TODO check for \n etc, allowed are alpha and so on
	
	//std::string str(bufv_.ptr(), i);
	//std::cout << "Lex string: " << str << std::endl;
	
	//into buffer
	reader_.copyto(*tok.buffer, reader_.pos(), i);
	reader_.skip(i+1); //skip "
	
	column_ += i;
}

void Lexer::lexComment(Token& tok)
{
	//look for doc comments
	//create doc comment tokens or 
}

inline void Lexer::nextChar()
{
	if(current() == '\n')
	{
		column_ = 1;
		line_++;
	}
		
	reader_.next<char>();
	column_++;
}

inline char& Lexer::current()
{
	return reader_.current<char>();
}

inline char& Lexer::peekChar(size_t n)
{
	return reader_.peek<char>(n);
}

inline void Lexer::skip(int i)
{
	reader_.skip(i);
	column_ += i;
}
	

inline void Lexer::checkForChar(Token& tok, char c, TokenId id)
{
	if(current() == c)
	{
		tok.id = id;
		nextChar();
	}
}

inline bool chkKw(const Token& tok, const char* kw)
{
	return *tok.buffer == kw;
}

inline void checkKeyword(Token& tok)
{
	if(chkKw(tok, "package")) { tok.id = TokenId::KwPackage; }
	else if(chkKw(tok, "import")) { tok.id = TokenId::KwImport; }
	else if(chkKw(tok, "def")) { tok.id = TokenId::KwDef; }
	else if(chkKw(tok, "obj")) { tok.id = TokenId::KwObj; }
	else if(chkKw(tok, "trait")) { tok.id = TokenId::KwTrait; }
	else if(chkKw(tok, "type")) { tok.id = TokenId::KwType; }
	else if(chkKw(tok, "var")) { tok.id = TokenId::KwVar; }
	else if(chkKw(tok, "let")) { tok.id = TokenId::KwLet; }
	else if(chkKw(tok, "const")) { tok.id = TokenId::KwConst; }
	else if(chkKw(tok, "if")) { tok.id = TokenId::KwIf; }
	else if(chkKw(tok, "else")) { tok.id = TokenId::KwElse; }
	else if(chkKw(tok, "for")) { tok.id = TokenId::KwFor; }
	else if(chkKw(tok, "while")) { tok.id = TokenId::KwWhile; }
	else if(chkKw(tok, "true")) { tok.id = TokenId::KwTrue; }
	else if(chkKw(tok, "false")) { tok.id = TokenId::KwFalse; }
	else if(chkKw(tok, "null")) { tok.id = TokenId::KwNull; }
	else if(chkKw(tok, "pub")) { tok.id = TokenId::KwPub; }
	else if(chkKw(tok, "priv")) { tok.id = TokenId::KwPriv; }
	else if(chkKw(tok, "prot")) { tok.id = TokenId::KwProt; }
	else if(chkKw(tok, "as")) { tok.id = TokenId::KwAs; }
}
