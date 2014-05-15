
#include <plf/utils/DotPrinter.hpp>

#include <plf/ast/Node.hpp>
#include <plf/ast/Declaration.hpp>
#include <plf/ast/Statement.hpp>
#include <plf/ast/Expression.hpp>
#include <plf/ast/Type.hpp>

using namespace plf;


/*
	ofstream myfile;
	myfile.open ("example.txt");
	myfile << "Writing this to a file.\n";
	myfile.close();
*/

void DotPrinter::generate(Crate& crate, const char* filename)
{
	out_.open(filename);
	//first run
	for(DeclPtr decl: crate.decls)
	{
		dispatch(decl);
	}
	out_.close();
}

void DotPrinter::visit(const Node& n)
{
	//should not be called
}

void DotPrinter::visit(const ModDecl& n)
{
	//when parent
	//mod loop through
	//name -> ....

}
