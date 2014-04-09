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
	return n;
}

NodePtr TypeResolver::visit(UseDecl& n)
{
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
	return n;
}

NodePtr TypeResolver::visit(InstanceDecl& n)
{
	return n;
}

// Statements =================================================================

NodePtr TypeResolver::visit(BlockStmt& n)
{
	return n;
}

NodePtr TypeResolver::visit(ReturnStmt& n)
{
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
	//dispatch(n.decl);

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

	return n;
}

NodePtr TypeResolver::visit(UnaryExpr& n)
{
	//dispatch(n.expr)
	return n;
}

NodePtr TypeResolver::visit(BinaryExpr& n)
{
	//take care of access operator
	// split out when access operator
	// require a right side step down

	//dispatch(n.left);
	//dispatch(n.right);

	return n;
}

NodePtr TypeResolver::visit(CallExpr& n)
{
	//has declaration?
	//n.params

	return n;
}


// Main =================================================================

void TypeResolver::run(Crate& crate)
{
	//first run
	for(DeclPtr decl: crate.decls)
	{
		dispatch(decl);
	}

	//required multiple runs?
	//require interpreter?
}
