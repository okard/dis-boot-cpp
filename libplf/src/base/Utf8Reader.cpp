
#include <plf/base/Utf8Reader.hpp>


#include <culcore/Assert.hpp>
#include <plf/base/FormatException.hpp>

using namespace plf;
using namespace cul;


static inline bool is_followbyte(unsigned char c)
{
	//10xxxxxx _> 1000 0000 & 1011 1111 80–BF
	return (c >> 6) == 2;
}


uchar Utf8Reader::read_uchar()
{
	lastPos_ = currentPos_;
	uchar c = buf_[currentPos_];

	//single char ascii
	if( c <= 0x7F) // 7bit info
	{
		currentPos_++;
		return c;
	}

	//two chars startbyte between 1100 0000  and 1101 1111
	// C0–C1 are illegal, -> C2–DF
	if( c >= 0xC2 && c <= 0xDF ) // has 5 + 6 bits info
	{
		assert(buf_.size() - currentPos_ > 1, "Not enough data in buffer");
		//check that the following byte has format 0x10xxxxxx
		assert((buf_[currentPos_+1] >> 6 ) == 2, "Follow byte has wrong format");

		uchar first = (buf_[currentPos_] & 0x1F) << 6;
		uchar second = (buf_[currentPos_+1] & 0x3F) << 0;
		c =  first | second;
		currentPos_ += 2; //skip 3 bytes
		return c;
	}

	//three chars E0–EF
	if( c >= 0xE0 && c <= 0xEF ) // has 4 + 6 + 6 bits info
	{
		assert(buf_.size() - currentPos_ > 2, "Not enough data in buffer");
		//check that the following byte has format 0x10xxxxxx
		assert((buf_[currentPos_+1] >> 6 ) == 2, "Follow byte has wrong format");
		assert((buf_[currentPos_+2] >> 6 ) == 2, "Follow byte has wrong format");

		uchar first = (buf_[currentPos_] & 0x0F) << 12;
		uchar second = (buf_[currentPos_+1] & 0x3F) << 6;
		uchar third = (buf_[currentPos_+2] & 0x3F) << 0;
		c =  first | second | third;
		currentPos_ += 3; //skip 3 bytes
		return c;
	}

	//four chars F0–F4
	// not valid F5–F7 (RFC 3629)
	if( c >= 0xF0 && c <= 0xF4 ) // has 3 + 6 + 6 + 6 bits info
	{
		assert(buf_.size() - currentPos_ > 3, "Not enough data in buffer");
		//check that the following byte has format 0x10xxxxxx
		assert((buf_[currentPos_+1] >> 6 ) == 2, "Follow byte has wrong format");
		assert((buf_[currentPos_+2] >> 6 ) == 2, "Follow byte has wrong format");
		assert((buf_[currentPos_+3] >> 6 ) == 2, "Follow byte has wrong format");

		uchar first = (buf_[currentPos_] & 0x07) << 18;
		uchar second = (buf_[currentPos_+1] & 0x3F) << 12;
		uchar third = (buf_[currentPos_+2] & 0x3F) << 6;
		uchar fourth = (buf_[currentPos_+3] & 0x3F) << 0;
		c =  first | second | third | fourth;
		currentPos_ += 4; //skip 4 bytes
		return c;
	}

	// 5 and 6 bytes length not valid anymore
	// 5 bytes F8–FB (RFC 3629)
	// 6 bytes FC–FD (RFC 3629)

	//FE–FF not defined

	throw FormatException("Invalid UTF8 sequence or not yet implemented byte: %x", c);
}

void Utf8Reader::reset()
{
	currentPos_ = 0;
}

size_t Utf8Reader::bytes_left() const
{
	if(currentPos_ > buf_.size())
		return 0;

	return  buf_.size() - currentPos_;
}
