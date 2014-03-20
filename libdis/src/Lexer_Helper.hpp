
#pragma once
#ifndef DIS_LEXER_HELPER_HPP
#define DIS_LEXER_HELPER_HPP

namespace {

//todo generic utf32 int as char

/// Is a whitespace character
inline static bool isWhitespace(char c)
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
inline static bool isAlpha(char c)
{
	return (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| c == '_';
}

//is c a numeric character
inline static bool isNumeric(char c)
{
	return (c >= '0' && c <= '9');
}

///is c a binary character
inline bool isBin(char c)
{
	return(c == '0' || c == '1');
}

///is c a hex character
inline static bool isHex(char c)
{
	return (c >= 'a' && c <= 'f')
		|| (c >= 'A' && c <= 'F')
		|| (c >= '0' && c <= '9');
}

//readable ascii: 0x21-0x7E



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
	for (std::size_t pos = size -1; pos != -1; pos-- )
	{
		hash = static_cast<unsigned int>(input[pos]) + ((hash << 5) + hash); /* hash * 33+ c*/
	}
	return hash;
}



}



#endif //DIS_LEXER_HELPER_HPP
