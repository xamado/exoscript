#include "ILByteCode.h"
#include <map>
#include "ILInstructionSet.h"

std::map<uint8_t, char*> gInstructionReadableName = {
	{ OP_HALT, "halt" },
	{ OP_JUMP, "jump" },
	{ OP_ADD, "add" },
	{ OP_SUBSTRACT, "sub" },
	{ OP_MULTIPLY, "mul" },
	{ OP_DIVIDE, "div" },
	{ OP_JUMP, "jump" },
	{ OP_JUMP, "jump" },
	{ OP_LOCAL_LOAD, "locld" },
	{ OP_LOCAL_STORE, "locst" },
	{ OP_PRINT, "print" },
	{ OP_LOCAL_STORE, "locst" },
	{ OP_LOCAL_LOAD, "locld" },
	{ OP_STACK_LOAD_INT, "sloadi" }
};

ILByteCode::ILByteCode()
{
	
}


ILByteCode::~ILByteCode()
{
}

void ILByteCode::WriteByte(uint8_t v)
{
	_data.push_back(v);
}

void ILByteCode::WriteInt(uint32_t v)
{
	_data.push_back((v >> 24) & 0xFF);
	_data.push_back((v >> 16) & 0xFF);
	_data.push_back((v >> 8) & 0xFF);
	_data.push_back(v & 0xFF);
}

void ILByteCode::WriteString(const std::string& s)
{
	for(uint32_t i = 0; i < s.length(); ++i)
	{
		_data.push_back(s[i]);
	}
}
