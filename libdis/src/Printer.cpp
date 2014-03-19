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
#include <plf/ast/Statement.hpp>
#include <plf/ast/Expression.hpp>

using namespace dis;
using namespace plf;


NodePtr Printer::visit(ModDecl& n, ParamPtr& arg)
{
	std::cout << "mod ";
	std::cout.write(n.name->ptr(), n.name->size());
	std::cout << std::endl << "{" << std::endl;
	
	//Iterate through declarations
	visitList<Declaration>(n.decls, arg);

	std::cout << "}" << std::endl;
	
	return n;
}

plf::NodePtr Printer::visit(plf::FunctionDecl& n, plf::ParamPtr& arg)
{
	//decl flags

	write(n.flags);

	std::cout << "def ";
	std::cout.write(n.name->ptr(), n.name->size());
	
	std::cout << "(";
	for(FunctionParameter& p: n.params)
	{
		std::cout.write(p.ident->ptr(), p.ident->size());
		std::cout << ", ";
	}
	std::cout << ")";
	std::cout << std::endl;
	
	if(n.body)
		n.body->accept(*this, arg);
	
	return n;
}

NodePtr Printer::visit(InstanceDecl& n, ParamPtr& arg)
{
	switch(n.itype)
	{
	case InstanceType::Constant:
		std::cout << "const ";
		break;
	case InstanceType::Value:
		std::cout << "let ";
		break;
	case InstanceType::Variable:
		std::cout << "var ";
		break;
	}

	std::cout.write(n.name->ptr(), n.name->size());

	//if has initializer write it
	if(n.init)
	{
		std::cout << " = ";
		n.init->accept(*this, arg);
	}

	std::cout << ";" << std::endl;

	return n;
}

void Printer::write(DeclFlags& flags)
{
	if((flags & DeclFlags::Private) == DeclFlags::Private)
		std::cout << "priv ";
	if((flags & DeclFlags::Public) == DeclFlags::Public)
		std::cout << "pub ";
	if((flags & DeclFlags::Protected) == DeclFlags::Protected)
		std::cout << "prot ";
}
