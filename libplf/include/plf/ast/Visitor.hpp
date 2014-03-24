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
#pragma once
#ifndef PLF_VISITOR_HPP
#define PLF_VISITOR_HPP

#include <plf/ast/Node.hpp>

namespace plf {
	
//Decl
class ModDecl;
class UseDecl;
class ClassDecl;
class TraitDecl;
class StructDecl;
class EnumDecl;
class AliasDecl;
class FunctionDecl;
class InstanceDecl;

//Stmt
class BlockStmt;
class ReturnStmt;
class ForStmt;
class WhileStmt;
class DeclStmt;
class ExprStmt; 

//Expr
class IntegerLiteral;
class FloatLiteral;
class HexLiteral;
class BinaryLiteral;
class StringLiteral;

class UnaryExpr;
class BinaryExpr;
class CallExpr;
class CastExpr;


//basic pattern with struct XXX { typedef xxx Type } Ref, ConstRef, Pure

//TODO more flexible type deductions? For Return Values/Parameter/...

template<typename T>
struct Ref
{
	using type = T&;
};

/**
 * Visitor Base
 */
template<typename R = void,
		 template<typename> class Wrapper = Ref,
		 typename... Args>
class VisitorBase
{
protected:

	//retrieve selected type for nodes
	template<typename T>
	using Type = typename Wrapper<T>::type;

public:

	virtual R visit(Type<Node> l, Args&... args) = 0;

	//Declarations------------------------------------------------------
	virtual R visit(Type<ModDecl> n, Args&... args)=0;
	virtual R visit(Type<UseDecl> n, Args&... args)=0;
	virtual R visit(Type<ClassDecl> n, Args&... args)=0;
	virtual R visit(Type<TraitDecl> n, Args&... args)=0;
	virtual R visit(Type<StructDecl> n, Args&... args)=0;

	virtual R visit(Type<EnumDecl> n, Args&... args)=0;
	virtual R visit(Type<AliasDecl> n, Args&... args)=0;
	virtual R visit(Type<FunctionDecl> n, Args&... args)=0;
	virtual R visit(Type<InstanceDecl> n, Args&... args)=0;

	//Statements--------------------------------------------------------
	//Expressions-------------------------------------------------------

};



//=======================================================================================
//Dispatch instead of visiting keep idea here

//class DispatchingVisitor : public VisitorBase

//	//cast helper for debug switch between casts
//	template<typename O>
//	static inline O cast(TypeDef<Node> in)
//	{
//		#ifndef NDEBUG
//			return dynamic_cast<O>(in);
//		#else
//			return static_cast<O>(in);
//		#endif
//	}


//for table dispatch
//	typedef Visitor<isConst, R, Args...> VisitorType;

//	template<class RT>
//	struct FuncType { };
//	template<class RT, class... ArgsF >
//	struct FuncType<RT(ArgsF...)>
//	{
//		typedef RT (*Type)(ArgsF...);
//	};

//	template<typename E>
//	constexpr auto to_integral(E e) -> typename std::underlying_type<E>::type
//	{
//	   return static_cast<typename std::underlying_type<E>::type>(e);
//	}


//	inline R dispatch(TypeDef<Node> n, Args&... args)
//	{
//		/*
//		static const typename FuncType<R(VisitorType*, TypeDef<Node>, Args&...)>::Type dispatch_table[to_integral(NodeKind::MAX)] =
//		{
//			[](VisitorType* lv, TypeDef<Node> ln, Args&... largs){return lv->visit(cast<TypeDef<Literal>>(ln), largs...);},
//			[](VisitorType* lv, TypeDef<Node> ln, Args&... largs){return lv->visit(cast<TypeDef<BinExpression>>(ln), largs...);}
//		};

//		return dispatch_table[to_integral(n.Kind)](this, n, args...);
//		*/

//		//return visit(cast<TypeDef<typename NodeTypeMap<n.Kind>::Type>>(n), args...);

//		//TODO use dynamic cast in debug
//		//create static function table to call with this? -> lambdas?
//		//can compiler optimise this?
//		switch(n.Kind)
//		{
//		case NodeKind::ModDecl:
//			return visit(cast<TypeDef<ModDecl>>(n), args...);
//		case NodeKind::UseDecl:
//			return visit(cast<TypeDef<UseDecl>>(n), args...);

//		default:
//			throw "error";
//		}

//	}


//	//implement twice for template parameter deduction

//	template<typename T>
//	inline R dispatch(std::shared_ptr<T>& n
//					 , Args&... args)
//	{
//		if(!n)
//		{
//			throw "error";
//		}
//		return dispatch(*(n.get()), args...);
//	}


//	template<typename T>
//	inline R dispatch(const std::shared_ptr<T>& n
//					 , Args&... args)
//	{
//		if(!n)
//		{
//			throw "error";
//		}
//		return dispatch(*(n.get()), args...);
//	}



/**
* Ast Visitor Class
*/
class Visitor : public VisitorBase<NodePtr, Ref, ParamPtr>
{

public:	

	//basic case
	virtual NodePtr visit(Node& n, ParamPtr& arg);

	//basic node types
	virtual NodePtr visit(Declaration& n, ParamPtr& arg);
	virtual NodePtr visit(Statement& n, ParamPtr& arg);
	virtual NodePtr visit(Expression& n, ParamPtr& arg);
	 
	//Declarations------------------------------------------------------
	virtual NodePtr visit(ModDecl& n, ParamPtr& arg);
	virtual NodePtr visit(UseDecl& n, ParamPtr& arg);
	virtual NodePtr visit(ClassDecl& n, ParamPtr& arg);
	virtual NodePtr visit(TraitDecl& n, ParamPtr& arg);
	virtual NodePtr visit(StructDecl& n, ParamPtr& arg);
	
	virtual NodePtr visit(EnumDecl& n, ParamPtr& arg);
	virtual NodePtr visit(AliasDecl& n, ParamPtr& arg);
	virtual NodePtr visit(FunctionDecl& n, ParamPtr& arg);
	virtual NodePtr visit(InstanceDecl& n, ParamPtr& arg);
	
	
	//Statements--------------------------------------------------------
	virtual NodePtr visit(BlockStmt& n, ParamPtr& arg);
	virtual NodePtr visit(ReturnStmt& n, ParamPtr& arg);
	virtual NodePtr visit(ForStmt& n, ParamPtr& arg);
	virtual NodePtr visit(WhileStmt& n, ParamPtr& arg);
	virtual NodePtr visit(DeclStmt& n, ParamPtr& arg);
	virtual NodePtr visit(ExprStmt& n, ParamPtr& arg);
	
	//Expressions-------------------------------------------------------
	virtual NodePtr visit(IntegerLiteral& n, ParamPtr& arg);
	virtual NodePtr visit(FloatLiteral& n, ParamPtr& arg);
	virtual NodePtr visit(HexLiteral& n, ParamPtr& arg);
	virtual NodePtr visit(BinaryLiteral& n, ParamPtr& arg);
	virtual NodePtr visit(StringLiteral& n, ParamPtr& arg);
	
	virtual NodePtr visit(UnaryExpr& n, ParamPtr& arg);
	virtual NodePtr visit(BinaryExpr& n, ParamPtr& arg);
	virtual NodePtr visit(CallExpr& n, ParamPtr& arg);
	
	//UnaryExpr
		//SubUnaryExpressions
	//BinaryExpr
		//SubBinaryExpressions
	//CallExpr
	
	//Types-------------------------------------------------------------


	//Helper
	template<class T>
	void visitList(PtrList<T>& list, ParamPtr& arg)
	{
		for(int i=0; list.size(); i++)
		{
			NodePtr n = list[i]->accept(*this, arg);
			list[i] = n->to<T>();
		}
	}
};


} //end namespace plf

#endif // PLF_VISITOR_HPP
