
#include <culcore/Test.hpp>
#include <culcore/Assert.hpp>

#include <plf/base/Buffer.hpp>

using namespace cul;
using namespace plf;


void Buffer_BasicTest()
{
	Buffer b = "test abc";
	assert(b[0] == 't');
	assert(b[4] == ' ');
	assert(b[7] == 'c');
	assert(b.size() == 8);
}

CUL_TEST(Buffer_BasicTest)


void Buffer_AllocTest()
{
	Buffer b;
	assert(b.size() == 0);
	b = "test";
	assert(b.size() == 4);
}
CUL_TEST(Buffer_AllocTest)


