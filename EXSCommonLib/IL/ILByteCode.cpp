#include "ILByteCode.h"

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
