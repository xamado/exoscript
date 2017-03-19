
#include "Compiler/BlockContext.h"
#include "util/Assert.h"

BlockContext::BlockContext(BlockContext* parentContext)
	: _parentContext(parentContext), _variableCount(0)
{
	_types.insert(std::make_pair("int", Type::Integer));
	_types.insert(std::make_pair("float", Type::Float));
}

BlockContext& BlockContext::PushContext()
{
	_subContexts.push_back(BlockContext(this));

	return _subContexts[_subContexts.size() - 1];
}

BlockContext::Type BlockContext::GetType(const std::string& name)
{
	auto i = _types.find(name);
	if(i != _types.end())
	{
		return i->second;
	}

	return Type::Invalid;
}

void BlockContext::AddVariable(BlockContext::ContextVariable& variable)
{
	variable.SetIndex(_variableCount++);

	_variables.insert(std::make_pair(variable.GetName(), variable));
}

const BlockContext::ContextVariable* BlockContext::GetVariable(const std::string& name) const
{
	auto it = _variables.find(name);
	if(it == _variables.end())
	{
		printf("Variable not found in context: %s\n", name.c_str());
		return nullptr;
	}

	return &it->second;
}

void BlockContext::AddLabel(const std::string& name, uint32_t offset)
{
	_offsets[name] = offset;
}

uint32_t BlockContext::GetLabel(const std::string& name)
{
	return _offsets[name];
}

void BlockContext::AddFunctionDeclaration(const Function& func)
{
	_functions.insert(std::pair<std::string, BlockContext::Function>(func.GetName(), func));
}

const BlockContext::Function* const BlockContext::GetFunction(const std::string& name) const
{
	auto iter = _functions.find(name);
	if (iter != _functions.end())
	{
		const BlockContext::Function* func = &iter->second;
		return func;
	}

	if (_parentContext != nullptr)
	{
		// Try finding in the parent context
		return _parentContext->GetFunction(name);
	}

	return nullptr;
}