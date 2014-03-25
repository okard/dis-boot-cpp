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

#include <plf/base/Buffer.hpp>

#include <plf/ast/Declaration.hpp>
#include <plf/ast/Statement.hpp>
#include <plf/ast/Expression.hpp>

using namespace dis;
using namespace plf;


NodePtr PrettyPrinter::visit(Node& n, ParamPtr& arg)
{
	return n;
}

NodePtr PrettyPrinter::visit(Declaration& n, ParamPtr& arg)
{
	return n;
}

NodePtr PrettyPrinter::visit(Statement& n, ParamPtr& arg)
{
	return n;
}

NodePtr PrettyPrinter::visit(Expression& n, ParamPtr& arg)
{
	return n;
}


NodePtr PrettyPrinter::visit(TraitDecl& n, ParamPtr& arg)
{
	return n;
}

NodePtr PrettyPrinter::visit(StructDecl& n, ParamPtr& arg)
{
	return n;
}

NodePtr PrettyPrinter::visit(EnumDecl& n, ParamPtr& arg)
{
	return n;
}

NodePtr PrettyPrinter::visit(AliasDecl& n, ParamPtr& arg)
{
	return n;
}


NodePtr PrettyPrinter::visit(ModDecl& n, ParamPtr& arg)
{
	std::cout << "mod ";
	write(n.name);
	std::cout << std::endl << "{" << std::endl;
	
	//Iterate through declarations
	visitList<Declaration>(n.decls, arg);

	std::cout << "}" << std::endl;
	
	return n;
}

NodePtr PrettyPrinter::visit(UseDecl& n, ParamPtr& arg)
{
	return n;
}

NodePtr PrettyPrinter::visit(ClassDecl& n, ParamPtr& arg)
{
	return n;
}

plf::NodePtr PrettyPrinter::visit(plf::FunctionDecl& n, plf::ParamPtr& arg)
{
	//decl flags

	write(n.flags);

	std::cout << "def ";
	write(n.name);
	
	std::cout << "(";
	for(FunctionParameter& p: n.params)
	{
		write(p.ident);
		std::cout << ", ";
	}
	std::cout << ")";
	std::cout << std::endl;
	
	if(n.body)
		n.body->accept(*this, arg);
	
	return n;
}

NodePtr PrettyPrinter::visit(InstanceDecl& n, ParamPtr& arg)
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

	write(n.name);

	//if has initializer write it
	if(n.init)
	{
		std::cout << " = ";
		n.init->accept(*this, arg);
	}

	std::cout << ";" << std::endl;

	return n;
}

NodePtr PrettyPrinter::visit(BlockStmt& n, ParamPtr& arg)
{
	for(auto stmt: n.statements)
		stmt->accept(*this, arg);
	return n;
}

NodePtr PrettyPrinter::visit(ReturnStmt& n, ParamPtr& arg)
{
	std::cout << "return ";
	n.expr->accept(*this, arg);
	std::cout << ";" << std::endl;
	return n;
}

NodePtr PrettyPrinter::visit(ForStmt& n, ParamPtr& arg)
{
	return n;
}

NodePtr PrettyPrinter::visit(WhileStmt& n, ParamPtr& arg)
{
	return n;
}

NodePtr PrettyPrinter::visit(DeclStmt& n, ParamPtr& arg)
{
	n.decl->accept(*this, arg);
	return n;
}

NodePtr PrettyPrinter::visit(ExprStmt& n, ParamPtr& arg)
{
	n.expr->accept(*this, arg);
	std::cout << std::endl;
	return n;
}

NodePtr PrettyPrinter::visit(IntegerLiteral& n, ParamPtr& arg)
{
	write(n.rawValue);
	return n;
}

NodePtr PrettyPrinter::visit(FloatLiteral& n, ParamPtr& arg)
{
	write(n.rawValue);
	return n;
}

NodePtr PrettyPrinter::visit(HexLiteral& n, ParamPtr& arg)
{
	write(n.rawValue);
	return n;
}

NodePtr PrettyPrinter::visit(BinaryLiteral& n, ParamPtr& arg)
{
	write(n.rawValue);
	return n;
}

NodePtr PrettyPrinter::visit(StringLiteral& n, ParamPtr& arg)
{
	write(n.rawValue);
	return n;
}

NodePtr PrettyPrinter::visit(IdentExpr& n, ParamPtr& arg)
{
	write(n.ident);
	return n;
}

NodePtr PrettyPrinter::visit(UnaryExpr& n, ParamPtr& arg)
{

	//prefix

	switch(n.op)
	{
	case UnaryOperator::LNot: std::cout << "!"; break;
	case UnaryOperator::Pos: std::cout << "+"; break;
	case UnaryOperator::Neg: std::cout << "-"; break;
	}

	n.expr->accept(*this, arg);

	//postfix


	return n;
}

NodePtr PrettyPrinter::visit(BinaryExpr& n, ParamPtr& arg)
{

	n.left->accept(*this, arg);

	switch(n.op)
	{
	case BinaryOperator::Assign: std::cout << "="; break;
	case BinaryOperator::Plus: std::cout << "+"; break;
	}

	n.right->accept(*this, arg);

	return n;
}

NodePtr PrettyPrinter::visit(CallExpr& n, ParamPtr& arg)
{
	return n;
}

void PrettyPrinter::write(plf::BufferPtr& buf)
{
	std::cout.write(buf->ptr(), buf->size());
}

void PrettyPrinter::write(DeclFlags& flags)
{
	if((flags & DeclFlags::Private) == DeclFlags::Private)
		std::cout << "priv ";
	if((flags & DeclFlags::Public) == DeclFlags::Public)
		std::cout << "pub ";
	if((flags & DeclFlags::Protected) == DeclFlags::Protected)
		std::cout << "prot ";
}
