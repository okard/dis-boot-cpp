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


void PrettyPrinter::visit(const Node& n)
{
	if(n.kind == NodeKind::Error)
		std::cerr << "Error Node" << std::endl;
	else
		std::cerr << "Unkown Node" << std::endl;
}

void PrettyPrinter::visit(const Declaration& n)
{
	//never called?
}

void PrettyPrinter::visit(const Statement& n)
{
	//never called?
}

void PrettyPrinter::visit(const Expression& n)
{
	//never called?
}


void PrettyPrinter::visit(const TraitDecl& n)
{
	std::cout << "trait [not yet implemented]" << std::endl;
}

void PrettyPrinter::visit(const StructDecl& n)
{
	std::cout << "struct [not yet implemented]" << std::endl;
}

void PrettyPrinter::visit(const EnumDecl& n)
{
	std::cout << "enum [not yet implemented]" << std::endl;
}

void PrettyPrinter::visit(const AliasDecl& n)
{
	std::cout << "alias [not yet implemented]" << std::endl;
}


void PrettyPrinter::visit(const ModDecl& n)
{
	std::cout << "mod ";
	write(n.name);

	if(n.decls.size() > 0)
	{
		std::cout << std::endl << "{" << std::endl;

		//Iterate through declarations
		for(DeclPtr decl: n.decls)
			dispatch(decl);

		std::cout << "}" << std::endl;
	}
	else
		std::cout << ";" << std::endl;
}

void PrettyPrinter::visit(const UseDecl& n)
{
	std::cout << "use [not yet implemented]" << std::endl;
}

void PrettyPrinter::visit(const ClassDecl& n)
{
	std::cout << "class [not yet implemented]" << std::endl;
}

void PrettyPrinter::visit(const plf::FunctionDecl& n)
{
	//decl flags

	write(n.flags);

	std::cout << "def ";
	write(n.name);
	
	std::cout << "(";
	for(const FunctionParameter& p: n.params)
	{
		write(p.ident);
		std::cout << ", ";
	}
	std::cout << ")";

	//look for body and type
	if(n.body)
	{
		if(n.body->kind == NodeKind::ExprStmt)
			std::cout << " = " ;
		else
			std::cout << std::endl;

		dispatch(n.body);
	}
	else
		std::cout << std::endl;
}

void PrettyPrinter::visit(const InstanceDecl& n)
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

	//TODO type


	//if has initializer write it
	if(n.init)
	{
		std::cout << " = ";
		dispatch(n.init);
	}

	std::cout << ";" << std::endl;
}

void PrettyPrinter::visit(const BlockStmt& n)
{
	for(StmtPtr stmt: n.statements)
		dispatch(stmt);
}

void PrettyPrinter::visit(const ReturnStmt& n)
{
	std::cout << "return ";
	dispatch(n.expr);
	std::cout << ";" << std::endl;
}

void PrettyPrinter::visit(const ForStmt& n)
{
	std::cout << "for [not yet implemented]" << std::endl;
}

void PrettyPrinter::visit(const WhileStmt& n)
{
	std::cout << "while [not yet implemented]" << std::endl;
}

void PrettyPrinter::visit(const DeclStmt& n)
{
	dispatch(n.decl);
}

void PrettyPrinter::visit(const ExprStmt& n)
{
	dispatch(n.expr);
	std::cout << ";" << std::endl;
}

void PrettyPrinter::visit(const IntegerLiteral& n)
{
	write(n.rawValue);
}

void PrettyPrinter::visit(const FloatLiteral& n)
{
	write(n.rawValue);
}

void PrettyPrinter::visit(const HexLiteral& n)
{
	write("0x");
	write(n.rawValue);
}

void PrettyPrinter::visit(const BinaryLiteral& n)
{
	write("0b");
	write(n.rawValue);
}

void PrettyPrinter::visit(const StringLiteral& n)
{
	write("\"");
	write(n.rawValue);
	write("\"");
}

void PrettyPrinter::visit(const IdentExpr& n)
{
	write(n.ident);
}

void PrettyPrinter::visit(const UnaryExpr& n)
{

	//prefix

	switch(n.op)
	{
	case UnaryOperator::LNot: std::cout << "!"; break;
	case UnaryOperator::Pos: std::cout << "+"; break;
	case UnaryOperator::Neg: std::cout << "-"; break;
	case UnaryOperator::Ref: std::cout << "&"; break;
	case UnaryOperator::PreIncr: std::cout << "++"; break;
	case UnaryOperator::PreDecr: std::cout << "--"; break;
	default: break;
	}

	dispatch(n.expr);

	//postfix
	switch(n.op)
	{
	case UnaryOperator::PostIncr: std::cout << "++"; break;
	case UnaryOperator::PostDecr: std::cout << "--"; break;
	default: break;
	}

}

void PrettyPrinter::visit(const BinaryExpr& n)
{

	dispatch(n.left);

	switch(n.op)
	{
	case BinaryOperator::Assign: std::cout << " = "; break;
	//Arithmetic
	case BinaryOperator::Plus: std::cout << " + "; break;
	case BinaryOperator::Minus: std::cout << " - "; break;
	case BinaryOperator::Mul: std::cout << " * "; break;
	case BinaryOperator::Div: std::cout << " / "; break;
	case BinaryOperator::Mod: std::cout << " % "; break;
	case BinaryOperator::Pow: std::cout << " ^^ "; break;

	//Arithmetic Compound Assign
	case BinaryOperator::PlusAssign: std::cout << " += "; break;
	case BinaryOperator::MinusAssign: std::cout << " -= "; break;
	case BinaryOperator::MulAssign: std::cout << " *= "; break;
	case BinaryOperator::DivAssign: std::cout << " /= "; break;
	case BinaryOperator::ModAssign: std::cout << " %= "; break;
	case BinaryOperator::PowAssign: std::cout << " ^^= "; break;

	default:
		std::cout << " [?] ";
	}

	dispatch(n.right);

}

void PrettyPrinter::visit(const CallExpr& n)
{
}

void PrettyPrinter::write(const char* str)
{
	std::cout << str;
}
void PrettyPrinter::writeln(const char* str)
{
	std::cout << str << std::endl;
}


void PrettyPrinter::write(const plf::BufferPtr& buf)
{
	std::cout.write(buf->ptr(), buf->size());
}


void PrettyPrinter::write(const DeclFlags& flags)
{
	if((flags & DeclFlags::Private) == DeclFlags::Private)
		write("priv ");
	if((flags & DeclFlags::Public) == DeclFlags::Public)
		std::cout << "pub ";
	if((flags & DeclFlags::Protected) == DeclFlags::Protected)
		std::cout << "prot ";
}
