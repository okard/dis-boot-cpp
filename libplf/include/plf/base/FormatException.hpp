
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
#ifndef PLF_FORMATEXCEPTION_HPP
#define PLF_FORMATEXCEPTION_HPP

#include <exception>

namespace plf {
	
/**
* Engine Exception
*/
class FormatException : public std::exception
{
private:
	static const int BUFSIZE = 1024;
    char msg_[BUFSIZE];

public:
    /**
    * Constructor
    */
    FormatException(const char* msg, ...);
    
    /**
    * Constructor
    */
    FormatException(int bufsize, const char* msg, ...);
    
    /**
    * Destructor
    */
    ~FormatException();
    
    /**
    * Description
    */
    virtual const char* what() const throw();
};

} // end namespace plf

#endif //PLF_FORMATEXCEPTION_HPP
