#pragma once

#include <string>

#include "IL/ILByteCode.h"

class Compiler
{
public:
	Compiler();
	~Compiler();

	ILByteCode* CompileScript(const std::string& filename);
};
