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
#include <plf/sem/TypeResolver.hpp>

#include <plf/base/FormatException.hpp>

#include <plf/ast/Declaration.hpp>
#include <plf/ast/Statement.hpp>
#include <plf/ast/Expression.hpp>

using namespace plf;


NodePtr TypeResolver::visit(Node& n)
{
	//error in typeresolver
	return n;
}

// Declarations =================================================================

NodePtr TypeResolver::visit(ModDecl& n)
{
	//loop through


	return n;
}

NodePtr TypeResolver::visit(UseDecl& n)
{
	//imports check?
	return n;
}

NodePtr TypeResolver::visit(ClassDecl& n)
{
	return n;
}

NodePtr TypeResolver::visit(TraitDecl& n)
{
	return n;
}

NodePtr TypeResolver::visit(StructDecl& n)
{
	return n;
}

NodePtr TypeResolver::visit(EnumDecl& n)
{
	return n;
}

NodePtr TypeResolver::visit(AliasDecl& n)
{
	return n;
}

NodePtr TypeResolver::visit(FunctionDecl& n)
{
	//instances?

	return n;
}

NodePtr TypeResolver::visit(InstanceDecl& n)
{
	//n.name
	//n.type
	//n.init

	return n;
}

// Statements =================================================================

NodePtr TypeResolver::visit(BlockStmt& n)
{
	for(unsigned int i=0; i < n.statements.size(); i++)
	{
		n.statements[i] = dispatch(n.statements[i])->to<Statement>();
	}
	return n;
}

NodePtr TypeResolver::visit(ReturnStmt& n)
{
	//solve expr
	n.expr = dispatch(n.expr)->to<Expression>();

	//check for function return type
	//parent seeking

	return n;
}

NodePtr TypeResolver::visit(ForStmt& n)
{
	return n;
}

NodePtr TypeResolver::visit(WhileStmt& n)
{
	return n;
}

NodePtr TypeResolver::visit(DeclStmt& n)
{
	n.decl = dispatch(n.decl)->to<Declaration>();

	return n;
}

NodePtr TypeResolver::visit(ExprStmt& n)
{
	//require return type
	n.expr = dispatch(n.expr)->to<Expression>();

	if(n.expr->returnType
	|| n.expr->returnType->kind == TypeKind::UnsolvedType
	|| n.expr->returnType->kind == TypeKind::UnkownType)
	{
		throw FormatException("Expr in ExprStmt has not a valid type");
	}

	return n;
}

// Expression =================================================================

NodePtr TypeResolver::visit(IntegerLiteral& n)
{
	//check for overflows? format?
	return n;
}

NodePtr TypeResolver::visit(FloatLiteral& n)
{
	// istrstream str(data,20)
	// str >> fnum;
	// if(str.fail())  { cout << "input fail\n"; exit(1); }

	return n;
}

NodePtr TypeResolver::visit(HexLiteral& n)
{
	return n;
}

NodePtr TypeResolver::visit(BinaryLiteral& n)
{
	return n;
}

NodePtr TypeResolver::visit(StringLiteral& n)
{
	return n;
}

NodePtr TypeResolver::visit(IdentExpr& n)
{
	//type resolving
	//pay attention to subtyping

	//current scope

	//check parent

	return n;
}

NodePtr TypeResolver::visit(UnaryExpr& n)
{
	n.expr = dispatch(n.expr)->to<Expression>();

	return n;
}

NodePtr TypeResolver::visit(BinaryExpr& n)
{
	//take care of access operator
	// split out when access operator
	// require a right side step down

	n.left = dispatch(n.left)->to<Expression>();
	n.right = dispatch(n.right)->to<Expression>();

	//check ops
	//check left and right result

	//if(n.left->returnType->kind == TypeKind::PrimaryType)

	//rewrite to operator calls?

	return n;
}

NodePtr TypeResolver::visit(CallExpr& n)
{
	//has declaration?
	//n.params

	//n.decl_expr

	return n;
}


// Main =================================================================

void TypeResolver::run(Crate& crate)
{
	//first run
	for(DeclPtr decl: crate.decls)
	{
		decl = dispatch(decl)->to<Declaration>();
	}

	//required multiple runs?
	//require interpreter?
}
