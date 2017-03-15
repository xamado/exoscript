#include "ILTextWriter.h"
#include "IL/ILInstructionSet.h"

#include <fstream>
#include <string>

ILTextWriter::ILTextWriter()
{
}


ILTextWriter::~ILTextWriter()
{
}

void ILTextWriter::WriteIL(ILByteCode* code)
{
	const uint8_t* data = code->GetByteCode();
	uint32_t len = code->GetByteCodeLength();
	const uint8_t* end = data + len;

	uint8_t* ptr = const_cast<uint8_t*>(data);

	while(ptr < end)
	{
		uint8_t instruction;
		ptr = ReadByte(ptr, &instruction);

		switch (instruction)
		{
			case OP_METADATA:
			{
				// read metadata type
				uint8_t metaType = 0;
				ptr = ReadByte(ptr, &metaType);

				switch (metaType)
				{
					case META_FUNCTION:
						ptr = this->WriteMetadataFunction(ptr);
						break;

					default:
						printf("Invalid metadata type read... this is bad!\n");
						break;
				}

				break;
			}		

			case OP_STACK_LOAD_INT:
			{
				uint32_t val = 0;
				ptr = ReadInt(ptr, &val);

				this->WriteOpCode(instruction, std::to_string(val));

				break;
			}

			case OP_LOCAL_STORE:
			{
				uint8_t val = 0;
				ptr = ReadByte(ptr, &val);

				this->WriteOpCode(instruction, std::to_string(val));

				break;
			}
			case OP_LOCAL_LOAD:
			{
				uint8_t val = 0;
				ptr = ReadByte(ptr, &val);

				this->WriteOpCode(instruction, std::to_string(val));

				break;
			}

			case OP_HALT:
			case OP_JUMP:
			case OP_ADD:
			case OP_SUBSTRACT:
			case OP_MULTIPLY:
			case OP_DIVIDE:
			case OP_PRINT:
				this->WriteOpCode(instruction);
				break;

			default:
				fs << "<UNKNOWN>" + std::string("\n");
				break;
		}
	}

	fs.close();
}

uint8_t* ILTextWriter::ReadByte(uint8_t* ptr, uint8_t* byte)
{
	*byte = *ptr;
	return ptr + 1;
}

uint8_t* ILTextWriter::ReadInt(uint8_t* ptr, uint32_t* val)
{
	*val = (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];

	return ptr + 4;
}

uint8_t* ILTextWriter::WriteMetadataFunction(uint8_t* ptr)
{
	uint8_t nameLen = 0;
	ptr = ReadByte(ptr, &nameLen);

	char* name = (char*)malloc(sizeof(char) * (nameLen + 1));
	name[nameLen] = '\0';

	for (uint32_t i = 0; i < nameLen; i++)
	{
		uint8_t c = 0;
		ptr = ReadByte(ptr, &c);

		name[i] = c;
	}

	fs << "function" << " " << name << ":" << "\n";

	free(name);

	return ptr;
}

void ILTextWriter::WriteOpCode(uint8_t op)
{
	std::string opCodeName = gInstructionReadableName[op];
	
	fs << opCodeName << "\n";
}

void ILTextWriter::WriteOpCode(uint8_t op, const std::string& data)
{
	std::string opCodeName = gInstructionReadableName[op];

	fs << opCodeName << "\t" << data << "\n";
}

void ILTextWriter::Open(const std::string& name)
{
	fs.open(name);
}

void ILTextWriter::Close()
{
	fs.close();
}
