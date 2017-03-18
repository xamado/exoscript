
#include "Compiler/BlockContext.h"
#include "util/Assert.h"

BlockContext::BlockContext(BlockContext* parentContext)
	: _parentContext(parentContext)
{

}

BlockContext& BlockContext::PushContext()
{
	_subContexts.push_back(BlockContext(this));

	return _subContexts[_subContexts.size() - 1];
}

void BlockContext::AddLocal(const std::string& id, const std::string& type)
{
	_locals.push_back(id);
}

bool BlockContext::GetLocal(const std::string& name, uint8_t* const val)
{
	for (uint32_t i = 0; i < _locals.size(); i++)
	{
		if (_locals[i] == name)
		{
			ASSERT_FATAL(i >= 0 && i < UINT8_MAX, "BlockContext::GetLocal() -> local index is out of range\n");

			*val = i;
			return true;
		}
	}

	printf("Variable not found in context: %s\n", name.c_str());

	return false;
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