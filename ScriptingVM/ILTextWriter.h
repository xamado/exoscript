#pragma once

#include "IL/ILByteCode.h"

#include <fstream>
#include "IL/ILInstructionSet.h"

class ILTextWriter
{
public:
	ILTextWriter(ILByteCode* code);
	~ILTextWriter();

	void WriteIL();

	uint8_t ReadByte();
	uint32_t ReadInt();
	float ReadFloat();

	void WriteMetadataHeader();
	void WriteMetadataFunction();
	void WriteOpCode(uint8_t  op);
	void WriteLineNumber();
	void WriteOpCode(uint8_t  op, const std::string& data);

	void Open(const std::string& name);

	void Close();

private:
	std::ofstream _fs;

	const uint8_t* _ptr;
	const uint8_t* _start;
	const uint8_t* _end;

	ILByteCode* _byteCode;

	uint8_t _indent = 0;
};

