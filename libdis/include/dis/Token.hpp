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
#ifndef __DIS_TOKEN_HPP__
#define __DIS_TOKEN_HPP__

#include <plf/base/Buffer.hpp>
#include <plf/base/Location.hpp>

//Macros for Token Generationen
#undef TOKEN
#define DIS_TOKEN 								\
		TOKEN(Unkown) 							\
		TOKEN(NotInitialized)					\
		TOKEN(Eof) 								\
		/* LITERALE */ 							\
		TOKEN(Ident) 							\
		TOKEN(IntLiteral) 						\
		TOKEN(FloatLiteral) 					\
		TOKEN(HexLiteral) 						\
		TOKEN(BinaryLiteral) 					\
		TOKEN(StringLiteral)					\
		/* SYMBOL */ 							\
		TOKEN(COBracket) 				/* { */ \
		TOKEN(CCBracket)				/* } */ \
		TOKEN(ROBracket)				/* ( */ \
		TOKEN(RCBracket)				/* ) */ \
		TOKEN(SOBracket)				/* [ */ \
		TOKEN(SCBracket)				/* ] */ \
		TOKEN(Dot)						/* . */ \
		TOKEN(Comma)					/* , */ \
		TOKEN(Colon)					/* : */ \
		TOKEN(Semicolon)				/* ; */ \
		TOKEN(Assign) 					/* = */	\
		TOKEN(EPoint)					/* ! */ \
		TOKEN(Sharp)					/* # */ \
		TOKEN(Tilde)					/* ~ */ \
		TOKEN(At)						/* @ */ \
		TOKEN(Plus)						/* + */ \
		TOKEN(Minus)					/* - */ \
		TOKEN(Mul)						/* * */ \
		TOKEN(Div)						/* / */ \
		TOKEN(Mod) 						/* % */ \
		TOKEN(Less)					    /* < */ \
		TOKEN(Greater)                  /* > */ \
		TOKEN(And)						/* & */ \
		TOKEN(Or)						/* | */ \
		TOKEN(Dollar)					/* $ */ \
		TOKEN(ColonColon)				/* :: */ \
		TOKEN(DotDot)				   /* .. */ \
		TOKEN(Shebang)				   /* #! */ \
		TOKEN(AttrStart)			   /* #[ */ \
		TOKEN(ConstraintStart)         /* [[ */ \
		TOKEN(ConstraintEnd)           /* ]] */ \
		TOKEN(TildeAssign)			   /* ~= */ \
		TOKEN(Equal)				   /* == */ \
		TOKEN(NEqual)				   /* != */ \
		TOKEN(PlusAssign)			   /* += */ \
		TOKEN(MinusAssign)			   /* -= */ \
		TOKEN(MulAssign)			   /* *= */ \
		TOKEN(DivAssign)			   /* /= */ \
		TOKEN(ModAssign)			   /* %= */ \
		TOKEN(GTE)					   /* >= */ \
		TOKEN(LTE)					   /* <= */ \
		TOKEN(ShiftL)				   /* << */ \
		TOKEN(ShiftR)				   /* >> */ \
		TOKEN(LAnd)					   /* && */ \
		TOKEN(LOr)					   /* || */ \
		TOKEN(PlusPlus)				   /* ++ */ \
		TOKEN(MinusMinus)			   /* -- */ \
		TOKEN(Lambda)				   /* -> */ \
		TOKEN(DollarDollar)			   /* $$ */ \
		TOKEN(DotDotDot)			  /* ... */ \
		TOKEN(DocMultipleStart)		 /* / *** */ \
		TOKEN(DocSingleStart)		  /* /// */ \
		/* KEYWORDS DECLARATION */ 				\
		TOKEN(KwMod)							\
		TOKEN(KwUse)							\
		TOKEN(KwUnsafe)							\
		TOKEN(KwTrait)							\
		TOKEN(KwType)							\
		TOKEN(KwDef) 							\
		TOKEN(KwStruct)							\
		TOKEN(KwEnum)							\
		TOKEN(KwVar) 							\
		TOKEN(KwLet) 							\
		TOKEN(KwConst)							\
		TOKEN(KwStatic)							\
		TOKEN(KwIf)								\
		TOKEN(KwElse)							\
		TOKEN(KwMatch)							\
		TOKEN(KwFor)							\
		TOKEN(KwWhile)							\
		TOKEN(KwReturn)							\
		TOKEN(KwTrue)							\
		TOKEN(KwFalse)							\
		TOKEN(KwPub)							\
		TOKEN(KwPriv)							\
		TOKEN(KwProt)							\
		TOKEN(KwAs)								

namespace dis {
	
/**
* TokenIds
*/
enum class TokenId
{
	#define TOKEN(x) x,
	DIS_TOKEN
	#undef TOKEN
};

/**
* Dis Lexer Token
*/
struct Token
{
	TokenId id = TokenId::NotInitialized;
	//to share one extracted string in nodes token uses a BufferPtr instead of Buffer
	plf::BufferPtr buffer = std::make_shared<plf::Buffer>();
	plf::Location loc;
};

//plf::SharedPtr<std::string>

const char* toString(TokenId id);


} //end namespace dis

#endif // __DIS_TOKEN_HPP__

