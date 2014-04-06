
#pragma once
#ifndef PLF_LLVMCODEGEN
#define PLF_LLVMCODEGEN

#include <llvm/IR/LLVMContext.h>


namespace plf {

class Crate;


/**
* @brief The LLVM Code Generator for PLF
*/
class LLVMCodeGen
{
private:
	llvm::LLVMContext context_;

public:
	LLVMCodeGen();

	void init_args(int argc, char *argv[]);

	void compile(Crate& crate);

};


}

#endif // PLF_LLVMCODEGEN
