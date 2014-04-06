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
#ifndef DIS_HELPER_HPP
#define DIS_HELPER_HPP

namespace {

/**
 * @brief return for size_t the value as hash
 * (hashes nothing)
 */
struct hash_dummy
{
  std::size_t operator()(std::size_t t) const
  {
	return t;
  }
};

/*
djb2 hash algo

 hash = ((hash << 5) + hash) + c;
		 hash * 33           + c;
*/

/**
 * @brief const_hash a compile time hashing function for string literals
 * @param input
 */
unsigned constexpr const_hash(char const *input)
{
	return *input ?
		static_cast<unsigned int>(*input) + 33 * const_hash(input + 1) :
		5381;
}

/**
 * @brief hash implement same algorithmus as const_hash for runtime and with size info
 * @param input
 * @param size
 */
unsigned hash(char const *input, std::size_t size)
{
	unsigned hash = 5381;
	for (std::size_t pos = size -1; pos != -1; pos-- ) //warning is ok here
	{
		hash = static_cast<unsigned int>(input[pos]) + ((hash << 5) + hash); /* hash * 33+ c*/
	}
	return hash;
}

}



#endif // DIS_HELPER_HPP
