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
#include <cultest/Test.hpp>
#include <culcore/Assert.hpp>

#include <plf/base/Utf8Reader.hpp>
#include <plf/base/FormatException.hpp>

#include <iostream>

using namespace cul;
using namespace plf;


constexpr uchar operator"" _uchar ( unsigned long long int i )
{
	return i;
}


constexpr uchar operator"" _uchar ( char c )
{
	return c;
}


void UtfReader_BasicTest()
{
	Utf8Reader r;

	//t ä € <violin clef>
	r.buf() = "t \xC3\xA4 \xE2\x82\xAC \xF0\x9D\x84\x9E";
	assert(r.buf().size() == 13);

	assert_eq<FormatException>(r.read_uchar(), 't'_uchar);
	assert_eq<FormatException>(r.read_uchar(), ' '_uchar);
	assert_eq<FormatException>(r.read_uchar(), 0x00E4_uchar); //ä 2 byte utf8
	assert_eq<FormatException>(r.read_uchar(), ' '_uchar);
	assert_eq<FormatException>(r.read_uchar(), 0x20AC_uchar); //€ 3 byte utf8
	assert_eq<FormatException>(r.read_uchar(), ' '_uchar);
	assert_eq<FormatException>(r.read_uchar(), 0x1D11E_uchar); //violin clef 4 byte utf8
}

CUL_TEST(UtfReader_BasicTest)
