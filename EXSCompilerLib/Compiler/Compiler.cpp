#include "Compiler/Compiler.h"

#include "Compiler/Lexer.h"
#include "Compiler/Parser.tab.h"
#include "Compiler/ILGenerator.h"
#include "Compiler/ASTNode.h"

#include "IL/ILByteCode.h"

extern ASTBlockNode* gProgramBlock;

Compiler::Compiler()
{
}


Compiler::~Compiler()
{
}

ILByteCode* Compiler::CompileScript(const std::string & filename)
{
	// Run the parser, it will use the lexer and output the AST in gProgramBlock
	FILE* fp;
	fopen_s(&fp, filename.c_str(), "rt");

	if(fp == nullptr)
	{
		printf("Failed to load script file: %s\n", filename.c_str());
		return nullptr;
	}

	// Attempt to parse, output will be stored in gProgramBlock (thanks flex/bison for the ugly)
	yyin = fp;
	yyparse();

	// Parse the AST
	ILGenerator generator;
	ILByteCode* code = generator.ParseAST(gProgramBlock);
	
	// Return a script object
	return code;
}
