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
#pragma once
#ifndef DIS_PRINTER_HPP
#define DIS_PRINTER_HPP

#include <plf/ast/Visitor.hpp>

namespace plf {
	class Buffer;
	typedef std::shared_ptr<Buffer> BufferPtr;
	enum class DeclFlags : unsigned char;
}

namespace dis {
	
/**
* Source Printer Visitor
* Pretty Printer
*/ 
class PrettyPrinter : public plf::ReadVisitor
{
private:
	//print to out buffer
	//tabs/idention
	//some options

public:

	//basic case
	virtual void visit(const plf::Node& n);

	//basic node types
	virtual void visit(const plf::Declaration& n);
	virtual void visit(const plf::Statement& n);
	virtual void visit(const plf::Expression& n);

	//Declarations------------------------------------------------------
	virtual void visit(const plf::ModDecl& n);
	virtual void visit(const plf::UseDecl& n);
	virtual void visit(const plf::ClassDecl& n);
	virtual void visit(const plf::TraitDecl& n);
	virtual void visit(const plf::StructDecl& n);

	virtual void visit(const plf::EnumDecl& n);
	virtual void visit(const plf::AliasDecl& n);
	virtual void visit(const plf::FunctionDecl& n);
	virtual void visit(const plf::InstanceDecl& n);


	//Statements--------------------------------------------------------
	virtual void visit(const plf::BlockStmt& n);
	virtual void visit(const plf::ReturnStmt& n);
	virtual void visit(const plf::ForStmt& n);
	virtual void visit(const plf::WhileStmt& n);
	virtual void visit(const plf::DeclStmt& n);
	virtual void visit(const plf::ExprStmt& n);

	//Expressions-------------------------------------------------------
	virtual void visit(const plf::IntegerLiteral& n);
	virtual void visit(const plf::FloatLiteral& n);
	virtual void visit(const plf::HexLiteral& n);
	virtual void visit(const plf::BinaryLiteral& n);
	virtual void visit(const plf::StringLiteral& n);

	virtual void visit(const plf::IdentExpr& n);
	virtual void visit(const plf::UnaryExpr& n);
	virtual void visit(const plf::BinaryExpr& n);
	virtual void visit(const plf::CallExpr& n);

private:
	void write(const char* str);
	void writeln(const char* str);
	void write(const plf::BufferPtr& buf);
	void write(const plf::DeclFlags& flags);

	//write functions for formatted strings/buffer
	//write(const char* msg, ...)
	//write(const BufferPtr& buf);
	//ident //tabs++
	//unident //tabs--
};
	
}

#endif // DIS_PRINTER_HPP
