#pragma once

#include <vector>
#include <map>

class BlockContext
{
public:
	enum Type
	{
		Void = 0,
		Integer = 1
	};

	class Function
	{
	public:
		Function()
			: _name(""), _parameterTypes(), _offset(0)
		{
			
		}

		Function(const std::string& name, const std::vector<Type>& types, uint32_t offset)
			: _name(name), _parameterTypes(types), _offset(offset)
		{

		}

		const std::string& GetName() const { return _name; }
		uint32_t GetOffset() const { return _offset; }
		
		uint32_t GetParameterCount() const { return _parameterTypes.size(); }

	private:
		const std::string _name;
		const std::vector<Type> _parameterTypes;
		const uint32_t _offset;
	};

public:
	void AddLocal(const std::string& id, const std::string& type);
	uint8_t GetLocal(const std::string& name);

	void AddLabel(const std::string& name, uint32_t offset);
	uint32_t GetLabel(const std::string& name);

	void AddFunctionDeclaration(const Function& func);
	const BlockContext::Function* const GetFunction(const std::string& name) const;

private:
	std::vector<std::string> _locals;
	std::map<std::string, uint32_t> _offsets;
	std::map<std::string, BlockContext::Function> _functions;
};
