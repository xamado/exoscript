#include "Compiler/ILGenerator.h"
#include "Compiler/ASTNode.h"
#include "IL/ILByteCode.h"

ILGenerator::ILGenerator()
{
}


ILGenerator::~ILGenerator()
{
}

ILByteCode* ILGenerator::ParseAST(ASTNode* node) const
{
	// TODO: ideally this should receive a stream (memory or file) to write to

	ILByteCode* bytecode = new ILByteCode();

	bytecode->WriteByte(InstructionSet::OP_STACK_LOAD_INT);
	bytecode->WriteInt(11);
	bytecode->WriteByte(InstructionSet::OP_STACK_LOAD_INT);
	bytecode->WriteInt(0);
	bytecode->WriteByte(InstructionSet::OP_JUMP);
	bytecode->WriteByte(InstructionSet::OP_HALT);

	BlockContext globalContext;
	bool status = node->GenerateCode(globalContext, bytecode);

	if(!status)
	{
		printf("Compilation aborted!\n");

		delete bytecode;
		delete node;

		return nullptr;
	}

	//uint32_t entryPointOffset = globalContext.GetLabel("main");
	const BlockContext::Function* const entryPoint = globalContext.GetFunction("main");
	uint32_t entryPointOffset = entryPoint->GetOffset();

	bytecode->_data[6] = ((entryPointOffset >> 24) & 0xFF);
	bytecode->_data[7] = ((entryPointOffset >> 16) & 0xFF);
	bytecode->_data[8] = ((entryPointOffset >> 8) & 0xFF);
	bytecode->_data[9] = ((entryPointOffset) & 0xFF);

	return bytecode;
}
