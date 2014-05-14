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

#ifndef NDEBUG
	#include <iostream>
	#include <typeinfo>
#endif


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

class IdentExpr;
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

template<typename T>
struct ConstRef
{
	using type = const T&;
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
	virtual R visit(Type<BlockStmt> n, Args&... args)=0;
	virtual R visit(Type<ReturnStmt> n, Args&... args)=0;
	virtual R visit(Type<ForStmt> n, Args&... args)=0;
	virtual R visit(Type<WhileStmt> n, Args&... args)=0;
	virtual R visit(Type<DeclStmt> n, Args&... args)=0;
	virtual R visit(Type<ExprStmt> n, Args&... args)=0;



	//Expressions-------------------------------------------------------
	virtual R visit(Type<IntegerLiteral> n, Args&... args)=0;
	virtual R visit(Type<FloatLiteral> n, Args&... args)=0;
	virtual R visit(Type<HexLiteral> n, Args&... args)=0;
	virtual R visit(Type<BinaryLiteral> n, Args&... args)=0;
	virtual R visit(Type<StringLiteral> n, Args&... args)=0;


	virtual R visit(Type<IdentExpr> n, Args&... args)=0;
	virtual R visit(Type<UnaryExpr> n, Args&... args)=0;
	virtual R visit(Type<BinaryExpr> n, Args&... args)=0;
	virtual R visit(Type<CallExpr> n, Args&... args)=0;

};



//=======================================================================================
//Dispatch instead of visiting keep idea here

template<typename R = void,
		 template<typename> class Wrapper = Ref,
		 typename... Args>
class DispatchVisitor : public VisitorBase<R, Wrapper, Args...>
{
private:

	//retrieve selected type for nodes
	template<typename T>
	using Type = typename Wrapper<T>::type;

	//cast helper for debug switch between casts
	template<typename Out>
	static inline Out cast(Type<Node> in)
	{
		return reinterpret_cast<Out>(in);
		//not working?
//		#ifndef NDEBUG
//			return dynamic_cast<Out>(in);
//		#else
//			return static_cast<Out>(in);
//		#endif
	}

public:

	//Options for dispatch
	// 1. switch
	// 2. dispatch table

	inline R dispatch(Type<Node> n, Args&... args)
	{
		switch(n.kind)
		{
		//Declarations
		case NodeKind::ModDecl: return this->visit(cast<Type<ModDecl>>(n), args...);
		case NodeKind::UseDecl: return this->visit(cast<Type<UseDecl>>(n), args...);
		case NodeKind::ClassDecl: return this->visit(cast<Type<ClassDecl>>(n), args...);
		case NodeKind::TraitDecl: return this->visit(cast<Type<TraitDecl>>(n), args...);
		case NodeKind::StructDecl: return this->visit(cast<Type<StructDecl>>(n), args...);
		case NodeKind::EnumDecl: return this->visit(cast<Type<EnumDecl>>(n), args...);
		case NodeKind::AliasDecl: return this->visit(cast<Type<AliasDecl>>(n), args...);
		case NodeKind::FunctionDecl: return this->visit(cast<Type<FunctionDecl>>(n), args...);
		case NodeKind::InstanceDecl: return this->visit(cast<Type<InstanceDecl>>(n), args...);
		//Statements
		case NodeKind::BlockStmt: return this->visit(cast<Type<BlockStmt>>(n), args...);
		case NodeKind::ReturnStmt: return this->visit(cast<Type<ReturnStmt>>(n), args...);
		case NodeKind::ForStmt: return this->visit(cast<Type<ForStmt>>(n), args...);
		case NodeKind::WhileStmt: return this->visit(cast<Type<WhileStmt>>(n), args...);
		case NodeKind::DeclStmt: return this->visit(cast<Type<DeclStmt>>(n), args...);
		case NodeKind::ExprStmt: return this->visit(cast<Type<ExprStmt>>(n), args...);
		//Expressions
		case NodeKind::IntegerLiteral: return this->visit(cast<Type<IntegerLiteral>>(n), args...);
		case NodeKind::FloatLiteral: return this->visit(cast<Type<FloatLiteral>>(n), args...);
		case NodeKind::HexLiteral: return this->visit(cast<Type<HexLiteral>>(n), args...);
		case NodeKind::BinaryLiteral: return this->visit(cast<Type<BinaryLiteral>>(n), args...);
		case NodeKind::StringLiteral: return this->visit(cast<Type<StringLiteral>>(n), args...);

		case NodeKind::IdentExpr: return this->visit(cast<Type<IdentExpr>>(n), args...);
		case NodeKind::UnaryExpr: return this->visit(cast<Type<UnaryExpr>>(n), args...);
		case NodeKind::BinaryExpr: return this->visit(cast<Type<BinaryExpr>>(n), args...);
		case NodeKind::CallExpr: return this->visit(cast<Type<CallExpr>>(n), args...);


		default:
			#ifndef NDEBUG
				 std::cerr << "missing node type dispatch for: " << typeid(n).name() << std::endl;
			#endif
			this->visit(n, args...);
			throw "error";
		}
	}


	//for template deduction a const and non-const variant
	template<typename T>
	inline R dispatch(std::shared_ptr<T>& n, Args&... args)
	{
		if(!n)
			throw "error";
		Node& nn = *n.get();
		return dispatch(nn, args...);
	}


	template<typename T>
	inline R dispatch(const std::shared_ptr<T>& n, Args&... args)
	{
		if(!n)
			throw "error";
		const Node& nn = *n.get();
		return dispatch(nn, args...);
	}

};

//default visitor types:
extern template class DispatchVisitor<void, ConstRef>;
typedef DispatchVisitor<void, ConstRef> ReadVisitor;





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

//	}



} //end namespace plf

#endif // PLF_VISITOR_HPP
