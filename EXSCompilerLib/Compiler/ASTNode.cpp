#include "Compiler/ASTNode.h"
//#include "util/Assert.h"

bool ASTExpressionStatementNode::GenerateCode(BlockContext& context, ILByteCode* bytecode) const
{
	bool status = _expr.GenerateCode(context, bytecode);
	if(!status)
	{
		return false;
	}

	return true;
}

// TODO: Let's add a step previous to generate code to parse all the syntax and contexts...
// the identifier should be able to know what type it is for generating the code...
// otherwise i need to do lots of ugly calls to figure out the type

bool ASTIdentifierNode::GenerateCode(BlockContext& context, ILByteCode* bytecode) const
{
	const BlockContext::ContextVariable* var = context.GetVariable(_name);
	if(var == nullptr)
	{
		return false;
	}

	// Push the value at local #localIdx into the stack
	bytecode->WriteByte(InstructionSet::OP_LOCAL_LOAD);
	bytecode->WriteByte(var->GetIndex());

	return true;
}

bool ASTBlockNode::GenerateCode(BlockContext& context, ILByteCode* bytecode) const
{
	std::vector<const ASTStatementNode*>::const_iterator it;
	for (it = _statements.begin(); it != _statements.end(); ++it)
	{
		const ASTStatementNode* node = (*it);

		bool status = node->GenerateCode(context, bytecode);
		if(!status)
		{
			return false;
		}
	}

	return true;
}

bool ASTVariableDeclarationNode::GenerateCode(BlockContext& context, ILByteCode* bytecode) const
{
	BlockContext::Type type = context.GetType(_type.GetName());
	if(type == BlockContext::Type::Invalid)
	{
		printf("Invalid type: %s\n", _type.GetName().c_str());
		return false;
	}

	BlockContext::ContextVariable v = BlockContext::ContextVariable(_id.GetName(), type);
	context.AddVariable(v);

	if (_rhs != nullptr)
	{
		bool status = _rhs->GenerateCode(context, bytecode);
		if(!status)
		{
			return false;
		}

		// Store the top of the stack into local #localIdx
		const BlockContext::ContextVariable* var = context.GetVariable(_id.GetName());
		if(var == nullptr)
		{
			return false;
		}

		uint8_t localIdx = var->GetIndex();
		bytecode->WriteByte(InstructionSet::OP_LOCAL_STORE);
		bytecode->WriteByte(localIdx);
	}

	return true;
}

bool ASTFunctionParametersNode::GenerateCode(BlockContext& context, ILByteCode* bytecode) const
{
	uint8_t idx = 0;

	for(auto i : _variables)
	{
		BlockContext::ContextVariable var = BlockContext::ContextVariable(i->GetNameIdentifier().GetName(), context.GetType(i->GetTypeIdentifier().GetName()));
		context.AddVariable(var);

		// this could be backwards
		bytecode->WriteByte(InstructionSet::OP_LOCAL_STORE);
		bytecode->WriteByte(idx);

		idx++;
	}

	return true;
}

bool ASTFunctionArgumentsNode::GenerateCode(BlockContext& context, ILByteCode* bytecode) const
{
	// Push our arguments on the stack
	for (auto i : _arguments)
	{
		bool status = (*i).GenerateCode(context, bytecode);
		if(!status)
		{
			return false;
		}
	}

	return true;
}

bool ASTFunctionDeclarationNode::GenerateCode(BlockContext& parentContext, ILByteCode* bytecode) const
{
	// Add a function declaration to the execution context
	std::vector<BlockContext::Type> parameterTypes;
	for (auto i : _parameters.GetParameters())
	{
		const ASTIdentifierNode& typeIdentifier = i->GetTypeIdentifier();
		const BlockContext::Type type = parentContext.GetType(typeIdentifier.GetName());
		parameterTypes.push_back(type);
	}

	uint32_t offset = bytecode->GetByteCodeLength();
	parentContext.AddFunctionDeclaration(BlockContext::Function(_name, parameterTypes, offset));

	// Push a new context
	BlockContext& context = parentContext.PushContext();

	// Write metadata about our function
	bytecode->WriteByte(OP_METADATA);
	bytecode->WriteByte(META_FUNCTION);
	bytecode->WriteByte(_name.length());
	bytecode->WriteString(_name);

	// Read the return address
	bytecode->WriteByte(InstructionSet::OP_LOCAL_STORE);
	bytecode->WriteByte(255);

	// Read the parameters from the stack
	bool status;
	status = _parameters.GenerateCode(context, bytecode);
	if(!status)
	{
		return false;
	}

	// Write the actual bytecode
	status = _block.GenerateCode(context, bytecode);
	if(!status)
	{
		return false;
	}

	bytecode->WriteByte(InstructionSet::OP_LOCAL_LOAD);
	bytecode->WriteByte(255);

	// In the case of the entry point let's use a HALT at the end instead of a return
	if(_name == "main")
	{
		bytecode->WriteByte(InstructionSet::OP_HALT);
	}
	else
	{
		bytecode->WriteByte(InstructionSet::OP_RETURN);
	}

	return true;
}

bool ASTFunctionCallNode::GenerateCode(BlockContext& context, ILByteCode* bytecode) const
{
	const BlockContext::Function* const func = context.GetFunction(_identifier.GetName());

	if(_arguments.GetArgumentCount() != func->GetParameterCount())
	{
		printf("Wrong number of arguments supplied: %s\n", func->GetName().c_str());
		return false;
	}

	uint8_t jumpLocation = func->GetOffset();

	// Push the jump-back location unto the stack
	//bytecode->WriteByte(InstructionSet::OP_STACK_LOAD_INT);
	//bytecode->WriteInt(currOffset + 3);
	
	// Push the arguments to the stack
	bool status = _arguments.GenerateCode(context, bytecode);
	if(!status)
	{
		return false;
	}

	// Push the jump-back location unto the stack
	bytecode->WriteByte(InstructionSet::OP_STACK_LOAD_INT);
	uint8_t currOffset = bytecode->GetByteCodeLength();
	bytecode->WriteInt(currOffset + 6);

	// Jump to offset
	bytecode->WriteByte(InstructionSet::OP_JUMP);
	bytecode->WriteByte(jumpLocation);

	return true;
}

bool ASTAssignmentNode::GenerateCode(BlockContext& context, ILByteCode* bytecode) const
{
	bool status = _rhs.GenerateCode(context, bytecode);
	if(!status)
	{
		return false;
	}

	context.GetVariable(_lhs.GetName());

	const BlockContext::ContextVariable* var = context.GetVariable(_lhs.GetName());
	if(var == nullptr)
	{
		return false;
	}

	uint8_t localIdx = var->GetIndex();
	bytecode->WriteByte(InstructionSet::OP_LOCAL_STORE);
	bytecode->WriteByte(localIdx);

	return true;
}

bool ASTBinaryOperatorNode::GenerateCode(BlockContext& context, ILByteCode* bytecode) const
{
	bool status;
	
	status = rhs.GenerateCode(context, bytecode);
	if(!status)
	{
		return false;
	}

	status = lhs.GenerateCode(context, bytecode);
	if(!status)
	{
		return false;
	}

	switch (op)
	{
	case '+':
		if (lhs.GetType(context) == BlockContext::Type::Integer)
		{
			bytecode->WriteByte(InstructionSet::OP_ADD_INT);
		}
		else
		{
			bytecode->WriteByte(InstructionSet::OP_ADD_FLOAT);
		}
		break;

	case '-':
		if (lhs.GetType(context) == BlockContext::Type::Integer)
		{
			bytecode->WriteByte(InstructionSet::OP_SUBSTRACT_INT);
		}
		else
		{
			bytecode->WriteByte(InstructionSet::OP_SUBSTRACT_FLOAT);
		}
		break;

	case '*':
		if (lhs.GetType(context) == BlockContext::Type::Integer)
		{
			bytecode->WriteByte(InstructionSet::OP_MULTIPLY_INT);
		}
		else
		{
			bytecode->WriteByte(InstructionSet::OP_MULTIPLY_FLOAT);
		}
		break;

	case '/':
		if (lhs.GetType(context) == BlockContext::Type::Integer)
		{
			bytecode->WriteByte(InstructionSet::OP_DIVIDE_INT);
		}
		else
		{
			bytecode->WriteByte(InstructionSet::OP_DIVIDE_FLOAT);
		}
		break;

	default:
		printf("Invalid operator: %c\n", op);
		break;
	}

	return true;
}

bool ASTIntegerNode::GenerateCode(BlockContext& context, ILByteCode* bytecode) const
{
	bytecode->WriteByte(InstructionSet::OP_STACK_LOAD_INT);
	bytecode->WriteInt(_value);
	
	return true;
}

bool ASTDoubleNode::GenerateCode(BlockContext& context, ILByteCode* bytecode) const
{
	bytecode->WriteByte(InstructionSet::OP_STACK_LOAD_FLOAT);
	bytecode->WriteFloat(_value);

	return true;
}

bool ASTPrintNode::GenerateCode(BlockContext& context, ILByteCode* bytecode) const
{
	bool status = _expression.GenerateCode(context, bytecode);
	if(!status)
	{
		return false;
	}

	bytecode->WriteByte(InstructionSet::OP_PRINT);

	return true;
}
