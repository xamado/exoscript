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

	bytecode->WriteByte(InstructionSet::OP_METADATA);
	bytecode->WriteByte(MetadataType::META_HEADER);
	bytecode->WriteByte(0); // entry point offset
		
	BlockContext globalContext(nullptr);
	bool status = node->GenerateCode(globalContext, bytecode);

	if(!status)
	{
		printf("Compilation aborted!\n");

		delete bytecode;
		delete node;

		return nullptr;
	}

	// Modify the entry point now that we know the offset
	const BlockContext::Function* const entryPoint = globalContext.GetFunction("main");
	uint8_t entryPointOffset = entryPoint->GetOffset();
	bytecode->_data[2] = entryPointOffset;

	return bytecode;
}
