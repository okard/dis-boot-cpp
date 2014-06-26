#include <culcore/Test.hpp>
#include <culcore/Assert.hpp>

#include <plf/base/Utf8Reader.hpp>
#include <plf/base/FormatException.hpp>

#include <iostream>

using namespace cul;
using namespace plf;


void UtfReader_BasicTest()
{
	Utf8Reader r;


	//t ä € <violin clef>

	r.buf() = "t \xC3\xA4 \xE2\x82\xAC \xF0\x9D\x84\x9E ";
	assert(r.buf().size() == 14);

	assert_eq<FormatException, uchar>(r.read_uchar(), 't');
	assert_eq<FormatException, uchar>(r.read_uchar(), ' ');
	assert_eq<FormatException, uchar>(r.read_uchar(), 0x00E4); //ä
	assert_eq<FormatException, uchar>(r.read_uchar(), ' ');
	assert_eq<FormatException, uchar>(r.read_uchar(), 0x20AC); //€
	assert_eq<FormatException, uchar>(r.read_uchar(), ' ');
	assert_eq<FormatException, uchar>(r.read_uchar(), 0x1D11E); //violin clef
	//r.read_uchar();
}

CUL_TEST(UtfReader_BasicTest)
