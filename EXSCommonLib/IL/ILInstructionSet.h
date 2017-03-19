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
	OP_STACK_LOAD_FLOAT,

	OP_LOCAL_STORE = 0x30,
	OP_LOCAL_LOAD,
	
	OP_ADD_INT = 0x40,
	OP_SUBSTRACT_INT,
	OP_MULTIPLY_INT,
	OP_DIVIDE_INT,	

	OP_ADD_FLOAT,
	OP_SUBSTRACT_FLOAT,
	OP_MULTIPLY_FLOAT,
	OP_DIVIDE_FLOAT
};

enum MetadataType
{
	META_HEADER = 0x1,
	META_FUNCTION = 0x2,
};

extern std::map<uint8_t, char*> gInstructionReadableName;
