
#include "Compiler/BlockContext.h"
#include "util/Assert.h"

void BlockContext::AddLocal(const std::string& id, const std::string& type)
{
	_locals.push_back(id);
}

uint8_t BlockContext::GetLocal(const std::string& name)
{
	for (uint32_t i = 0; i < _locals.size(); i++)
	{
		if (_locals[i] == name)
		{
			ASSERT_FATAL(i >= 0 && i < UINT8_MAX, "BlockContext::GetLocal() -> local index is out of range\n");
			return i;
		}
	}

	// TODO: Handle errors gracefully here, -1 is not valid
	return -1;
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

	return nullptr;
}