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
#include <plf/ast/Visitor.hpp>

#include <plf/ast/Declaration.hpp>
#include <plf/ast/Statement.hpp>
#include <plf/ast/Expression.hpp>
#include <plf/ast/Type.hpp>
#include <plf/ast/Attribute.hpp>

using namespace plf;


namespace plf {
	template class DispatchVisitor<void, ConstRef>;
}


/*
example usage:

NodePtr Visitor::visit(PackageDecl& n, ParamPtr& arg)
{
	for (int i=0; i < n.Decl.size(); i++) 
	{
		n.Decl[i] = n.Decl[i].accept(*this, arg)->to<Declaration>; 
	}
	
	// DeclPtr d = Node::create<Declaration>();
	// return Node::create<Node>(d);
	 
	return n;
}
*/


NodePtr Visitor::visit(Node& n, ParamPtr& arg)
{
	//NodePtr n = Node::create<Node>();
	return n;
}

NodePtr Visitor::visit(Declaration& n, ParamPtr& arg)
{
	return n;
}

NodePtr Visitor::visit(Statement& n, ParamPtr& arg)
{
	return n;
}

NodePtr Visitor::visit(Expression& n, ParamPtr& arg)
{
	return n;
}

////////////////////////////////////////////////////////////////////////
// Declaration

NodePtr Visitor::visit(ModDecl& pd, ParamPtr& arg)
{
	//visit declarations
	for(unsigned int i = 0; i < pd.decls.size(); i++)
	{
		pd.decls[i] = pd.decls[i]->accept(*this, arg)->to<Declaration>();
	}
	
	return pd;
}

NodePtr Visitor::visit(UseDecl& n, ParamPtr& arg)
{
	
	return n;
}
	
NodePtr Visitor::visit(ClassDecl& n, ParamPtr& arg)
{
	return n;
}

NodePtr Visitor::visit(TraitDecl& n, ParamPtr& arg)
{
	return n;
}

NodePtr Visitor::visit(StructDecl& n, ParamPtr& arg)
{
	return n;
}

NodePtr Visitor::visit(EnumDecl& n, ParamPtr& arg)
{
	return n;
}

NodePtr Visitor::visit(AliasDecl& n, ParamPtr& arg)
{
	return n;
}

NodePtr Visitor::visit(FunctionDecl& n, ParamPtr& arg)
{
	return n;
}

NodePtr Visitor::visit(InstanceDecl& n, ParamPtr& arg)
{
	return n;
}

////////////////////////////////////////////////////////////////////////
// Stmt

NodePtr Visitor::visit(BlockStmt& n, ParamPtr& arg)
{
	return n;
}

NodePtr Visitor::visit(ReturnStmt& n, ParamPtr& arg)
{
	return n;
}

NodePtr Visitor::visit(ForStmt& n, ParamPtr& arg)
{
	return n;
}

NodePtr Visitor::visit(WhileStmt& n, ParamPtr& arg)
{
	return n;
}

NodePtr Visitor::visit(DeclStmt& n, ParamPtr& arg)
{
	return n;
}

NodePtr Visitor::visit(ExprStmt& n, ParamPtr& arg)
{
	return n;
}

////////////////////////////////////////////////////////////////////////
// Expr

NodePtr Visitor::visit(IntegerLiteral& n, ParamPtr& arg)
{
	return n;
}

NodePtr Visitor::visit(FloatLiteral& n, ParamPtr& arg)
{
	return n;
}

NodePtr Visitor::visit(HexLiteral& n, ParamPtr& arg)
{
	return n;
}

NodePtr Visitor::visit(BinaryLiteral& n, ParamPtr& arg)
{
	return n;
}

NodePtr Visitor::visit(StringLiteral& n, ParamPtr& arg)
{
	return n;
}

NodePtr Visitor::visit(IdentExpr& n, ParamPtr& arg)
{
	return n;
}


NodePtr Visitor::visit(UnaryExpr& n, ParamPtr& arg)
{
	return n;
}

NodePtr Visitor::visit(BinaryExpr& n, ParamPtr& arg)
{
	return n;
}

NodePtr Visitor::visit(CallExpr& n, ParamPtr& arg)
{
	return n;
}


