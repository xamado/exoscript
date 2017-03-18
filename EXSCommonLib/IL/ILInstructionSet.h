#pragma once
#include <map>

enum InstructionSet
{
	OP_HALT = 0x0,

	OP_PRINT,
	OP_METADATA,

	OP_JUMP = 0x10,
	OP_RETURN,

	OP_STACK_LOAD_INT = 0x20,

	OP_LOCAL_STORE = 0x30,
	OP_LOCAL_LOAD,
	
	OP_ADD = 0x40,
	OP_SUBSTRACT,
	OP_MULTIPLY,
	OP_DIVIDE,	
};

enum MetadataType
{
	META_HEADER = 0x1,
	META_FUNCTION = 0x2,
};

extern std::map<uint8_t, char*> gInstructionReadableName;
