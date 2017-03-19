#pragma once

#include <stdint.h>
#include <vector>

class ILByteCode
{
public:
	ILByteCode();
	~ILByteCode();

	const uint8_t* const GetByteCode() const { return &_data[0]; } 
	uint32_t GetByteCodeLength() const { return _data.size(); } 

	void WriteByte(uint8_t v);
	void WriteInt(uint32_t v);
	void WriteFloat(float v);
	void WriteString(const std::string& s);

	bool IsValid() const { return _data.size() > 0; }

public:
	std::vector<uint8_t> _data;
};

