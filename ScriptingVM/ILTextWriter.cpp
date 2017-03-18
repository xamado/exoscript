#include "ILTextWriter.h"
#include "IL/ILInstructionSet.h"

#include <fstream>
#include <string>

ILTextWriter::ILTextWriter(ILByteCode* code)
{
	_byteCode = code;

	_ptr = code->GetByteCode();
	_start = _ptr;
	_end = _ptr + code->GetByteCodeLength();
}

ILTextWriter::~ILTextWriter()
{
}

void ILTextWriter::WriteIL()
{
	while(_ptr < _end)
	{
		this->WriteLineNumber();

		uint8_t instruction = ReadByte();

		switch (instruction)
		{
			case OP_METADATA:
			{
				// read metadata type
				uint8_t metaType = ReadByte();

				switch (metaType)
				{
					case META_HEADER:
						this->WriteMetadataHeader();
						break;
					case META_FUNCTION:
						this->WriteMetadataFunction();

						_indent++;

						break;			
					default:
						printf("Invalid metadata type read... this is bad!\n");
						break;
				}

				break;
			}		

			case OP_STACK_LOAD_INT:
			{
				uint32_t val = ReadInt();

				this->WriteOpCode(instruction, std::to_string(val));

				break;
			}

			case OP_LOCAL_STORE:
			{
				uint8_t val = ReadByte();

				this->WriteOpCode(instruction, std::to_string(val));

				break;
			}
			case OP_LOCAL_LOAD:
			{
				uint8_t val = ReadByte();

				this->WriteOpCode(instruction, std::to_string(val));

				break;
			}

			case OP_JUMP:
			{
				uint8_t address = ReadByte();
				
				this->WriteOpCode(instruction, std::to_string(address));

				break;
			}

			case OP_RETURN:
				this->WriteOpCode(instruction);
				_indent--;
				break;

			case OP_HALT:
			case OP_ADD:
			case OP_SUBSTRACT:
			case OP_MULTIPLY:
			case OP_DIVIDE:
			case OP_PRINT:
				this->WriteOpCode(instruction);

				break;

			default:
				_fs << "<UNKNOWN>" + std::string("\n");
				break;
		}
	}

	_fs.close();
}

uint8_t ILTextWriter::ReadByte()
{
	uint8_t b = *_ptr;
	_ptr++;

	return b;
}

uint32_t ILTextWriter::ReadInt()
{
	uint32_t v = (_ptr[0] << 24) | (_ptr[1] << 16) | (_ptr[2] << 8) | _ptr[3];
	_ptr += 4;

	return v;
}

void ILTextWriter::WriteLineNumber()
{
	_fs << (_ptr - _start) << ": ";
}

void ILTextWriter::WriteMetadataHeader()
{
	uint8_t entryPointOffset = ReadByte();

	_fs << "header" << ":" << "\n";

	_fs << "\t" << "entry point: " << std::to_string(entryPointOffset) << "\n";
}

void ILTextWriter::WriteMetadataFunction()
{
	uint8_t nameLen = ReadByte();

	char* name = (char*)malloc(sizeof(char) * (nameLen + 1));
	name[nameLen] = '\0';

	for (uint32_t i = 0; i < nameLen; i++)
	{
		uint8_t c = ReadByte();

		name[i] = c;
	}

	_fs << "function" << " " << name << ":" << "\n";

	free(name);
}

void ILTextWriter::WriteOpCode(uint8_t op)
{
	std::string opCodeName = gInstructionReadableName[op];
	
	for (uint32_t i = 0; i < _indent; ++i)
		_fs << "\t";

	_fs << opCodeName << "\n";
}

void ILTextWriter::WriteOpCode(uint8_t op, const std::string& data)
{
	std::string opCodeName = gInstructionReadableName[op];

	for (uint32_t i = 0; i < _indent; ++i)
		_fs << "\t";

	_fs << opCodeName << "\t" << data << "\n";
}

void ILTextWriter::Open(const std::string& name)
{
	_fs.open(name);
}

void ILTextWriter::Close()
{
	_fs.close();
}
