#pragma once

#include "IL/ILByteCode.h"

#include <fstream>
#include "IL/ILInstructionSet.h"

class ILTextWriter
{
public:
	ILTextWriter();
	~ILTextWriter();
	
	void WriteIL(ILByteCode* code);

	uint8_t* ReadByte(uint8_t* ptr, uint8_t* byte);

	uint8_t* ReadInt(uint8_t* ptr, uint32_t* val);

	uint8_t* WriteMetadataFunction(uint8_t* ptr);
	void WriteOpCode(uint8_t  op);
	void WriteOpCode(uint8_t  op, const std::string& data);

	void Open(const std::string& name);

	void Close();

private:
	std::ofstream fs;
};

