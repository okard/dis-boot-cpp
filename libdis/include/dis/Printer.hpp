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
class PrettyPrinter : public plf::Visitor
{
private:
	//print to out buffer
	//tabs/idention
	//some options

public:

	//basic case
	virtual plf::NodePtr visit(plf::Node& n, plf::ParamPtr& arg);

	//basic node types
	virtual plf::NodePtr visit(plf::Declaration& n, plf::ParamPtr& arg);
	virtual plf::NodePtr visit(plf::Statement& n, plf::ParamPtr& arg);
	virtual plf::NodePtr visit(plf::Expression& n, plf::ParamPtr& arg);

	//Declarations------------------------------------------------------
	virtual plf::NodePtr visit(plf::ModDecl& n, plf::ParamPtr& arg);
	virtual plf::NodePtr visit(plf::UseDecl& n, plf::ParamPtr& arg);
	virtual plf::NodePtr visit(plf::ClassDecl& n, plf::ParamPtr& arg);
	virtual plf::NodePtr visit(plf::TraitDecl& n, plf::ParamPtr& arg);
	virtual plf::NodePtr visit(plf::StructDecl& n, plf::ParamPtr& arg);

	virtual plf::NodePtr visit(plf::EnumDecl& n, plf::ParamPtr& arg);
	virtual plf::NodePtr visit(plf::AliasDecl& n, plf::ParamPtr& arg);
	virtual plf::NodePtr visit(plf::FunctionDecl& n, plf::ParamPtr& arg);
	virtual plf::NodePtr visit(plf::InstanceDecl& n, plf::ParamPtr& arg);


	//Statements--------------------------------------------------------
	virtual plf::NodePtr visit(plf::BlockStmt& n, plf::ParamPtr& arg);
	virtual plf::NodePtr visit(plf::ReturnStmt& n, plf::ParamPtr& arg);
	virtual plf::NodePtr visit(plf::ForStmt& n, plf::ParamPtr& arg);
	virtual plf::NodePtr visit(plf::WhileStmt& n, plf::ParamPtr& arg);
	virtual plf::NodePtr visit(plf::DeclStmt& n, plf::ParamPtr& arg);
	virtual plf::NodePtr visit(plf::ExprStmt& n, plf::ParamPtr& arg);

	//Expressions-------------------------------------------------------
	virtual plf::NodePtr visit(plf::IntegerLiteral& n, plf::ParamPtr& arg);
	virtual plf::NodePtr visit(plf::FloatLiteral& n, plf::ParamPtr& arg);
	virtual plf::NodePtr visit(plf::HexLiteral& n, plf::ParamPtr& arg);
	virtual plf::NodePtr visit(plf::BinaryLiteral& n, plf::ParamPtr& arg);
	virtual plf::NodePtr visit(plf::StringLiteral& n, plf::ParamPtr& arg);

	virtual plf::NodePtr visit(plf::IdentExpr& n, plf::ParamPtr& arg);
	virtual plf::NodePtr visit(plf::UnaryExpr& n, plf::ParamPtr& arg);
	virtual plf::NodePtr visit(plf::BinaryExpr& n, plf::ParamPtr& arg);
	virtual plf::NodePtr visit(plf::CallExpr& n, plf::ParamPtr& arg);

private:
	void write(plf::BufferPtr& buf);
	void write(plf::DeclFlags& flags);

	//write functions for formatted strings/buffer
	//write(const char* msg, ...)
	//write(const BufferPtr& buf);
	//ident //tabs++
	//unident //tabs--
};
	
}

#endif // DIS_PRINTER_HPP
