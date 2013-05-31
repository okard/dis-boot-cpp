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
//#include <unordered_map>

#include <plf/base/FormatException.hpp>

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

inline void checkKeyword(Token& tok);

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
	toklist_.clear();
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
	if(bufv_.eob())
	{
		tok.id = TokenId::Eof;
		return;
	}
	
	while(isWhitespace(bufv_.current<char>()))
		bufv_.next<char>();
		
	char c = bufv_.current<char>();
	
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
		case '{': tok.id = TokenId::COBracket; bufv_.next<char>(); break;
		case '}': tok.id = TokenId::CCBracket; bufv_.next<char>(); break;
		case '(': tok.id = TokenId::ROBracket; bufv_.next<char>(); break;
		case ')': tok.id = TokenId::RCBracket; bufv_.next<char>(); break;
		case '[': tok.id = TokenId::SOBracket; bufv_.next<char>(); break;
		case ']': tok.id = TokenId::SCBracket; bufv_.next<char>(); break;
		case '.': tok.id = TokenId::Dot; bufv_.next<char>(); break;
		case ',': tok.id = TokenId::Comma; bufv_.next<char>(); break;
		case ':': tok.id = TokenId::Colon; bufv_.next<char>(); break;
		case ';': tok.id = TokenId::Semicolon; bufv_.next<char>(); break;
		case '=': tok.id = TokenId::Assign; bufv_.next<char>(); break;
		case '@': tok.id = TokenId::At; bufv_.next<char>(); break;
		case '!': tok.id = TokenId::EPoint; bufv_.next<char>(); break;
		case '#': tok.id = TokenId::Sharp; bufv_.next<char>(); break;
		case '~': tok.id = TokenId::Tilde; bufv_.next<char>(); break;
		case '+': tok.id = TokenId::Plus; bufv_.next<char>(); break;
		case '-': tok.id = TokenId::Minus; bufv_.next<char>(); break;
		case '*': tok.id = TokenId::Mul; bufv_.next<char>(); break;
		case '/': tok.id = TokenId::Div; bufv_.next<char>(); break;
		case '%': tok.id = TokenId::Mod; bufv_.next<char>(); break;

		
		default: 
			//error unkown char
			throw plf::FormatException("Unknown Character: %c", c);
	}
	
	//check for comments
	if(tok.id == TokenId::Div && bufv_.current<char>() == '/')
	{
		while(!bufv_.eob() &&  bufv_.current<char>() != '\n')
			bufv_.next<char>();
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
		case TokenId::Sharp: checkForChar(tok, '!', TokenId::Shebang); break;
		case TokenId::EPoint: checkForChar(tok, '=', TokenId::NEqual); break;
		case TokenId::Assign: checkForChar(tok, '=', TokenId::Equal); break;
		case TokenId::Plus: checkForChar(tok, '=', TokenId::PlusAssign); break;
		case TokenId::Minus: checkForChar(tok, '=', TokenId::MinusAssign); break;
		case TokenId::Mul: checkForChar(tok, '=', TokenId::MulAssign); break;
		case TokenId::Div: checkForChar(tok, '=', TokenId::DivAssign); break;
		case TokenId::Mod: checkForChar(tok, '=', TokenId::ModAssign); break;
		case TokenId::Tilde: checkForChar(tok, '=', TokenId::TildeAssign); break;
	}
	
	//check triple token
	switch(tok.id)
	{
		case TokenId::DotDot: checkForChar(tok, '.', TokenId::TripleDot); break;
	}
}


void Lexer::lexId(Token& tok)
{
	tok.id = TokenId::Ident;
	
	int i;
	for(i = 0; isAlpha(bufv_.peek<char>(i)); i++); //TODO check for eob
	
	//read bufv_.ptr() size i into token
	//std::string str(bufv_.ptr(), i);
	//std::cout << "Lex id: " << str << std::endl;
	
	//add the id to token buffer
	tok.buffer = std::make_shared<plf::Buffer>(i);
	tok.buffer->insert(bufv_.ptr(), i);
	
	checkKeyword(tok);
	
	//skip over
	bufv_.set(i + bufv_.pos());
}

void Lexer::lexNumber(Token& tok)
{
	tok.id = TokenId::IntLiteral;
	
	//binary
	if(bufv_.current<char>() == '0' 
	&& bufv_.peek<char>(1) == 'b')
	{
		tok.id = TokenId::BinaryLiteral;
		bufv_.next<char>(); //now b
		bufv_.next<char>(); //now first digit
		
		int i;
		for(i = 0; isBin(bufv_.current<char>()); i++); //TODO check for eob
		tok.buffer = std::make_shared<plf::Buffer>(i);
		tok.buffer->insert(bufv_.ptr(), i);
		return;
	}
	
	//hex
	if(bufv_.current<char>() == '0' 
	&& bufv_.peek<char>(1) == 'x')
	{
		tok.id = TokenId::HexLiteral;
		bufv_.next<char>(); //now x
		bufv_.next<char>(); //now first digit
		
		int i;
		for(i = 0; isHex(bufv_.current<char>()); i++); //TODO check for eob
		tok.buffer = std::make_shared<plf::Buffer>(i);
		tok.buffer->insert(bufv_.ptr(), i);
		return;
	}
	
	//normal number
	//dot == double
	//only one dot allowed
	//if dot and ends with .f or .{num}f
	
	
	bufv_.next<char>(); 
}

void Lexer::lexString(Token& tok)
{
	//assert(bufv_.current<char> == '"');
	tok.id = TokenId::StringLiteral;
	
	bufv_.next<char>(); //skip "
	int i;
	for(i = 0; bufv_.peek<char>(i) != '"'; i++); //TODO check for eob
	
	//std::string str(bufv_.ptr(), i);
	//std::cout << "Lex string: " << str << std::endl;
	
	//into buffer
	tok.buffer = std::make_shared<plf::Buffer>(i);
	tok.buffer->insert(bufv_.ptr(), i);
	
	bufv_.set(i + bufv_.pos()+1); //skip " 
}

void Lexer::lexComment(Token& tok)
{
	//look for doc comments
	//create doc comment tokens or 
}


inline void Lexer::checkForChar(Token& tok, char c, TokenId id)
{
	if(bufv_.current<char>() == c)
	{
		tok.id = id;
		bufv_.next<char>();
	}
}

inline bool chkKw(const Token& tok, const char* kw)
{
	return 0 == memcmp(tok.buffer->ptr(), kw, tok.buffer->size());
}

inline void checkKeyword(Token& tok)
{
	if(chkKw(tok, "package")) { tok.id = TokenId::KwPackage; }
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
}
