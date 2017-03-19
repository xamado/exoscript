#pragma once

#include "IL/ILByteCode.h"

#include <cstdint>
#include <map>

class VM
{
public:
	VM();
	~VM();

	void ExecuteByteCode(const ILByteCode* const byteCode);

	static uint8_t* ReadByte(uint8_t* ptr, uint8_t* val);
	static uint8_t* ReadInt(uint8_t* ptr, uint32_t* val);
	uint8_t* ReadFloat(uint8_t* ptr, float* val);

	uint8_t* ReadMetadata(uint8_t* ptr);

	void Push(uint32_t value);
	uint32_t Pop();
	
	void PushFloat(float value);
	float PopFloat();

private:
	uint32_t _stack[1024];
	uint32_t _stackCounter;

	std::map<uint8_t, uint32_t> _locals;
};

