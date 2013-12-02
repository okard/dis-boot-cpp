/*
Dis Programming Language Frontend Library

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
#include <dis/Printer.hpp>

#include <iostream>

#include <plf/ast/Declaration.hpp>

using namespace dis;
using namespace plf;

NodePtr Printer::visit(PackageDecl& n, ParamPtr& arg)
{
	std::cout << "package ";
		for(auto p: n.path)
		{
			std::cout.write(p->ptr(), p->size());
			std::cout << '.';
		}
	std::cout << ";" << std::endl;
	
	//accept subnodes
	for(auto d: n.decls)
	{
		d->accept(*this, arg);
	}
	
	return n;
}

plf::NodePtr Printer::visit(plf::FunctionDecl& n, plf::ParamPtr& arg)
{
	//decl flags
	std::cout << "def ";
	std::cout.write(n.name->ptr(), n.name->size());
	std::cout << std::endl;
	
	return n;
}
