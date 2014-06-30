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
#include <unordered_map>

#include <culcore/Assert.hpp>
#include <plf/base/Exception.hpp>
#include <plf/base/FormatException.hpp>

#include <iostream>
#include <iomanip>

using namespace dis;
using namespace plf;
using namespace cul;

////////////////////////
// Helper
#include "Helper.hpp"
#include "Lexer_Helper.hpp"

namespace {

inline static void checkKeyword(Token& tok)
{
	static const std::unordered_map<std::size_t, TokenId, hash_dummy> keywordMap =
	{
		{ const_hash("mod"), TokenId::KwMod },
		{ const_hash("use"), TokenId::KwUse },
		{ const_hash("def"), TokenId::KwDef },
		{ const_hash("trait"), TokenId::KwTrait },
		{ const_hash("struct"), TokenId::KwStruct },
		{ const_hash("enum"), TokenId::KwEnum },
		{ const_hash("type"), TokenId::KwType},

		{ const_hash("var"), TokenId::KwVar },
		{ const_hash("let"), TokenId::KwLet },
		{ const_hash("const"), TokenId::KwConst },
		{ const_hash("static"), TokenId::KwStatic},

		{ const_hash("if"), TokenId::KwIf },
		{ const_hash("else"), TokenId::KwElse },
		{ const_hash("for"), TokenId::KwFor },
		{ const_hash("while"), TokenId::KwWhile },
		{ const_hash("match"), TokenId::KwMatch },
		{ const_hash("return"), TokenId::KwReturn },

		{ const_hash("true"), TokenId::KwTrue },
		{ const_hash("false"), TokenId::KwFalse },

		{ const_hash("pub"), TokenId::KwPub },
		{ const_hash("priv"), TokenId::KwPriv},
		{ const_hash("prot"), TokenId::KwProt },
		{ const_hash("unsafe"), TokenId::KwUnsafe },

		{ const_hash("as"), TokenId::KwAs }
	};

	auto it = keywordMap.find(hash(tok.buffer->str(), tok.buffer->size()));
	if(it != keywordMap.end())
	{
		tok.id=it->second;
	}
}

} //end anonymous namespace

Lexer::Lexer()
{
}

Lexer::~Lexer()
{
}

void Lexer::open(plf::SourcePtr srcptr)
{
	source_ = srcptr;
	reader_.buf().alloc(source_->size());
	source_->readComplete(reader_.buf());
	reader_.reset();
	nextChar(); //first char

	
	//TODO reset internal status
	column_ = 1;
	line_ = 1;
}


void Lexer::next(Token& token)
{
	//token.buffer = std::make_shared<Buffer>();
	token.buffer->free();
	token.id = TokenId::NotInitialized;
	lexToken(token);
}

void  Lexer::lexToken(Token& tok)
{	
	//eos is set after parsing the last valid token!
	if(reader_.is_eob())
	{
		tok.id = TokenId::Eof;
		tok.loc.column = column_;
		tok.loc.line = line_;
		return;
	}
	
	while(isWhitespace(curchar_))
	{
		nextChar();
		//can go eos here
		if(reader_.is_eob())
		{
			tok.id = TokenId::Eof;
			tok.loc.column = column_;
			tok.loc.line = line_;
			return;
		}
	}

	uchar c = curchar_;
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
		case '$': tok.id = TokenId::Dollar; nextChar(); break;
		

		//TODO FIXIT early eof
		/*case 0x00:
			//throw plf::FormatException("Nullchar");
			std::cout << "Found null l: " << line_ << std::endl;
			tok.id = TokenId::Eof;
			return;
		*/
		
		default: 
			//error unkown char
			throw plf::FormatException("(%d, %d) Unknown Character: %02x", line_, column_, c);
	}
	
	//check for comments
	if(tok.id == TokenId::Div && curchar_ == '/')
	{
		while(!reader_.is_eob())
		{
			nextChar();
			if(curchar_ == '\n')
			{
				break;
			}
		}
		lexToken(tok);
		return;
	}
	
	//TODO /* */ comments
	
	//TODO /** */ and /// doc comments
	
	//check double token
	switch(tok.id)
	{
		case TokenId::Colon: checkForChar(tok, ':', TokenId::ColonColon); break;
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
		case TokenId::Less: checkForChar(tok, '=', TokenId::LTE);
							checkForChar(tok, '<', TokenId::ShiftL);
							break;
		case TokenId::Greater:
							checkForChar(tok, '=', TokenId::GTE);
							checkForChar(tok, '>', TokenId::ShiftR);
							break;
		case TokenId::And: checkForChar(tok, '&', TokenId::LAnd); break;
		case TokenId::Or: checkForChar(tok, '|', TokenId::LOr); break;
		case TokenId::Dollar: checkForChar(tok, '$', TokenId::DollarDollar); break;
		default:break;
	}
	
	//check triple token
	switch(tok.id)
	{
		case TokenId::DotDot: checkForChar(tok, '.', TokenId::DotDotDot); break;
		default:break;
	}
}


void Lexer::lexId(Token& tok)
{
	tok.id = TokenId::Ident;
	
	size_t i = reader_.last_pos();

	while (isAlpha(curchar_) || isNumeric(curchar_))
	{
		nextChar();
	}
	
	//read bufv_.ptr() size i into token
	//std::string str(bufv_.ptr(), i);
	//std::cout << "Lex id: " << str << std::endl;
	
	//add the id to token buffer
	tok.buffer->set_from(reader_.buf(), i, reader_.last_pos());
	
	checkKeyword(tok);
}

void Lexer::lexNumber(Token& tok)
{
	tok.id = TokenId::IntLiteral;
	
	assert(isNumeric(curchar_), "Lexer::lexNumber not a numeric char");
	
	size_t start = reader_.last_pos();

	//binary
	if(curchar_ == '0')
	{
		nextChar();

		if(curchar_ == 'b')
		{
			nextChar(); //skip b
			tok.id = TokenId::BinaryLiteral;

			size_t start = reader_.last_pos();
			while(isBin(curchar_))
			{
				nextChar();
			}

			tok.buffer->set_from(reader_.buf(), start, reader_.last_pos());
			return;
		}

		if(curchar_ == 'x')
		{
			nextChar(); //skip x
			tok.id = TokenId::HexLiteral;

			size_t start = reader_.last_pos();
			while(isHex(curchar_)) {
				nextChar();
			}

			tok.buffer->set_from(reader_.buf(), start, reader_.last_pos());
			return;
		}

	}
	
	//eating numchars
	while(isNumeric(curchar_))
	{
		nextChar();
	}

	//if there is a dot recognize as floating point and read rest
	if(curchar_ == '.')
	{
		tok.id = TokenId::FloatLiteral;
		nextChar();
		while(isNumeric(curchar_))
		{
			nextChar();
		}
	}

	//create the token buffer
	tok.buffer->set_from(reader_.buf(), start, reader_.last_pos());
}

void Lexer::lexString(Token& tok)
{
	assert(curchar_ == '"', "Lexer::lexString: not a expected '\"'");
	tok.id = TokenId::StringLiteral;
	
	nextChar(); //skip "

	size_t start = reader_.last_pos();

	while(curchar_ != '"') {
		nextChar();
	}
	
	//TODO check for \n etc, allowed are alpha and so on
	
	//std::string str(bufv_.ptr(), i);
	//std::cout << "Lex string: " << str << std::endl;
	
	//into buffer
	tok.buffer->set_from(reader_.buf(), start, reader_.last_pos());

	nextChar(); //skip "
}

void Lexer::lexComment(Token& tok)
{
	//look for doc comments
	//create doc comment tokens or 
	throw plf::Exception("Lexer::lexComment not yet implemented");
}

inline void Lexer::nextChar()
{
	if(curchar_ == '\n')
	{
		column_ = 1;
		line_++;
	}
	curchar_ = reader_.read_uchar();
	column_++;
	//std::cout << "char: " << std::hex << reader_.current<char>() << std::endl;
}
	
//checks if the current token is c then set tok id to id and go to next char
inline void Lexer::checkForChar(Token& tok, uchar c, TokenId id)
{
	if(curchar_ == c)
	{
		tok.id = id;
		nextChar();
	}
}
