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
#ifndef PLF_ATTRIBUTE_HPP
#define PLF_ATTRIBUTE_HPP

#include <plf/ast/Node.hpp>

namespace plf {

//runtime attributes
//compile time attributes

/**
* Basic Attribute class 
* #[attr(param)]
*/
class Attribute : public Node //required to inherit from node?
{
};

/**
* define ABI for functions etc
* C, stdcall, fastcall
*/
class AbiAttribute : public Attribute
{
	//default
};

/**
* Test ABI
*/
class TestAttribute : public Attribute
{

};


/**
* @brief The UserAttribute class
*/
class UserAttribute : public Attribute
{

};


//attach attribute to functions
	// enter_function,
	// leave_function

//CustomAttribute

//Lang Attribute?
// #[lang("op:eq")]
// implement operators as functions
	//def opabcblabla(this: &foo, b: T)
	
} //end namespace plf

#endif //PLF_ATTRIBUTE_HPP
