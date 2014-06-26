/*
Programming Language Framework (PLF)

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
#ifndef PLF_ASSERT_HPP
#define PLF_ASSERT_HPP

#include <plf/base/Exception.hpp>

namespace plf {

//throw specific exception
template<typename T>
static inline void assert(bool expr, const char* msg)
{
	if(!expr)
		throw T(msg);
}


//basic assert macro
static inline void assert(bool expr, const char* msg)
{
	if(!expr)
		throw Exception(msg);
}


//basic assert macro (rely on optimization)
static inline void assertd(bool expr, const char* msg)
{
	#ifndef NDEBUG
	if(!expr)
		throw Exception(msg);
	#endif
}


}

#endif // PLF_ASSERT_HPP
