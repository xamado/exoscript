#include "VM.h"

#include "util/Assert.h"

#include "IL/ILByteCode.h"
#include "IL/ILInstructionSet.h"

#include <cstdint>
#include <cstdio>
#include <cstring>

VM::VM()
{
	_stackCounter = 0;
	memset(_stack, 0, sizeof(_stack));
}

VM::~VM()
{
}

#define DEBUG_INSTRUCTIONS 1

void VM::ExecuteByteCode(const ILByteCode* const bytecode)
{
	if (!bytecode->IsValid())
		return;

	uint8_t* ptr = const_cast<uint8_t*>(bytecode->GetByteCode());
	uint8_t* start = ptr;
	uint8_t* end = ptr + bytecode->GetByteCodeLength();

	// Read the header
	uint8_t val = 0;
	ptr = ReadByte(ptr, &val);
	ASSERT_FATAL(val == InstructionSet::OP_METADATA, "Header was invalid");

	ptr = ReadByte(ptr, &val);
	ASSERT_FATAL(val == MetadataType::META_HEADER, "Header was invalid");

	// Push the return location to be the next byte, which is always a halt
	Push(3);

	// Jump to the entry point
	ptr = ReadByte(ptr, &val);
	ptr = start + val;

	while(ptr < end)
	{
		uint8_t offset = (ptr - start);
		uint8_t instruction = 0;
		ptr = ReadByte(ptr, &instruction);

#ifdef DEBUG_INSTRUCTIONS
		printf("%d: %02X\n", offset, instruction);
#endif

		switch(instruction)
		{
			case OP_HALT:
				ptr = end;
				break;

			case OP_METADATA:
				ptr = ReadMetadata(ptr);
				break;

			case OP_JUMP:
			{
				uint8_t offset = 0;
				ptr = ReadByte(ptr, &offset);

				ptr = start + offset;

				break;
			}

			case OP_RETURN:
			{
				uint8_t offset = Pop();

				ptr = start + offset;
				
				break;
			}
			case OP_STACK_LOAD_INT:
			{
				uint32_t val = 0;
				ptr = ReadInt(ptr, &val);

				Push(val);

				break;
			}

			case OP_ADD:
			{
				uint32_t a = Pop();
				uint32_t b = Pop();

				uint32_t c = a + b;

				Push(c);
			}
			break;

			case OP_SUBSTRACT:
			{
				uint32_t a = Pop();
				uint32_t b = Pop();

				uint32_t c = a - b;

				Push(c);
			}
			break;

			case OP_MULTIPLY:
			{
				uint32_t a = Pop();
				uint32_t b = Pop();

				uint32_t c = a * b;

				Push(c);
			}
			break;

			case OP_DIVIDE:
			{
				uint32_t a = Pop();
				uint32_t b = Pop();

				uint32_t c = a / b;

				Push(c);
			}
			break;

			case OP_LOCAL_STORE:
			{
				uint8_t index;
				ptr = ReadByte(ptr, &index);

				uint32_t v = Pop();

				_locals[index] = v;
			}
			break;

			case OP_LOCAL_LOAD:
			{
				uint8_t index;
				ptr = ReadByte(ptr, &index);

				uint32_t v = _locals[index];

				Push(v);
			}
			break;

			case OP_PRINT:
			{
				uint32_t v = Pop();

				printf("print: %d\n", v);
				break;
			}

			default:
				printf("VM -> Unknown opcode: %02X\n", instruction);
				return;
				break;
		}
	}
}

uint8_t* VM::ReadByte(uint8_t* ptr, uint8_t* byte)
{
	*byte = *ptr;
	return ptr + 1;
}

uint8_t* VM::ReadInt(uint8_t* ptr, uint32_t* val)
{
	*val = (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];

	return ptr + 4;
}

uint8_t* VM::ReadMetadata(uint8_t* ptr)
{
	uint8_t metaType = 0;
	ptr = ReadByte(ptr, &metaType);

	switch(metaType)
	{
		case META_FUNCTION:
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

			free(name);

			break;
		}

		default:
			ASSERT_FATAL(false, "VM::ReadMetaData() -> Invalid metadata found in bytecode, unable to proceed parsing");
			break;
	}

	return ptr;
}

void VM::Push(uint32_t value)
{
	ASSERT_FATAL(_stackCounter < 1024, "VM::Push() -> Stack overflow");

	_stack[_stackCounter++] = value;
}

uint32_t VM::Pop()
{
	ASSERT_FATAL(_stackCounter > 0, "VM::Push() -> Stack is empty");

	return _stack[--_stackCounter];
}
