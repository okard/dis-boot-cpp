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
#ifndef PLF_SYMBOLTABLE_HPP
#define PLF_SYMBOLTABLE_HPP

#include <plf/base/External.hpp>
#include <plf/base/Buffer.hpp>

namespace plf {

enum class SymbolType
{
	Function,
	Data
};
	
/**
* A single (native) symbol
*/
class Symbol
{
	//NodePtr

	//global variables ( general global, static member, (TLS variables?))
	//functions

	//name(mangled?) BufferPtr (mangled name)
	//DeclPtr
	//extern elsewhere declared resolved by linking
	//static not exported symbols package internal only
	//use_counter //is referenced or not used
	//external type? (RTTI relevant)

	//symbol attribute
};


//inherit from symbol
//class FunctionSymbol : public Symbol { FunctionDecl& decl; pointer? }
//class DataSymbol : public Symbol { }

typedef SharedPtr<Symbol> SymbolPtr;

/**
* Complete (native) SymbolTable
* means a flat table with mangled names
*/
class SymbolTable
{
	//data/function split?

	//symbol list
	//map<mangled name, symbol>
	//list <symbol>
	
	Map<BufferPtr, SymbolPtr, hash_BufferPtr> symbols;
};
	
	
} //end namespace plf

#endif //PLF_SYMBOLTABLE_HPP
