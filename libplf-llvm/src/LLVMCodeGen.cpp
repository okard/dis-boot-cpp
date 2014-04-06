#include <plf/llvm/LLVMCodeGen.hpp>

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>

#include <plf/ast/Crate.hpp>

using namespace plf;


LLVMCodeGen::LLVMCodeGen()
{

}

void plf::LLVMCodeGen::init_args(int argc, char* argv[])
{

}

void LLVMCodeGen::compile(Crate& crate)
{
	// Make the module, which holds all the code.
	auto module = new llvm::Module("crate", context_);
	llvm::IRBuilder<> builder(context_);

	//loop through crate.symbolTable

	//FunctionType
	// Function

	//debug: module->dump () const

	//BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", TheFunction);
	//builder.SetInsertPoint(BB);

	//llvm::WriteBitcodeToFile(const Module *M, raw_ostream &Out)
}
