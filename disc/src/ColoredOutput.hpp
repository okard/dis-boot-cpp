/*
Dis Compiler

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
#ifndef DISC_COLOREDOUTPUT_HPP
#define DISC_COLOREDOUTPUT_HPP

#include <iostream>

//todo improve,  use class for saving default colors?


#ifdef WIN32
#include <windows.h>

inline std::ostream& blue(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE
              |FOREGROUND_GREEN|FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& red(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, 
                FOREGROUND_RED|FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& green(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, 
              FOREGROUND_GREEN|FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& yellow(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, 
         FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
    return s;
}

#else

/*
HEADER = '\033[95m'
OKBLUE = '\033[94m'
OKGREEN = '\033[92m'
WARNING = '\033[93m'
FAIL = '\033[91m'
ENDC = '\033[0m'
*/

inline std::ostream& blue(std::ostream &s)
{
	s << "\033[94m";
	return s;
}

inline std::ostream& red(std::ostream &s)
{
	s << "\033[91m";
	return s;
}

inline std::ostream& green(std::ostream &s)
{
	s << "\033[92m";
	return s;
}

inline std::ostream& yellow(std::ostream &s)
{
	s << "\033[93m";
	return s;
}

inline std::ostream& reset_color(std::ostream &s)
{
	s << "\033[0m";
	return s;
}

#endif


#endif // DISC_COLOREDOUTPUT_HPP
