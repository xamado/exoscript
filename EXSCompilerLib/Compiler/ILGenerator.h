#pragma once

#include "IL/ILByteCode.h"

#include <vector>
#include <map>

class ASTNode;
class ASTIdentifierNode;

class ILGenerator
{
public:
	ILGenerator();
	~ILGenerator();

	ILByteCode* ParseAST(ASTNode* node) const;
};

