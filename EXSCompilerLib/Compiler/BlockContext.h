#pragma once

#include <vector>
#include <map>

class BlockContext
{
public:
	enum Type
	{
		Invalid = 0,
		Void = 0x1,
		Integer = 0x2,
		Float = 0x3
	};

	class ContextVariable
	{
	public:
		ContextVariable()
			: _name(""), _type(), _index(0)
		{
			
		}

		ContextVariable(const std::string& name, const Type type)
			: _name(name), _type(type)
		{
			
		}

		const std::string& GetName() const { return _name; }
		
		Type GetType() const { return _type; }

		uint32_t GetIndex() const { return _index; };
		void SetIndex(uint32_t idx) { _index = idx; }

	private:
		const std::string _name;
		const Type _type;

		uint32_t _index;
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
	BlockContext(BlockContext* parentContext);

	BlockContext& PushContext();
	
	Type GetType(const std::string& name);

	void AddVariable(ContextVariable& variable);
	const BlockContext::ContextVariable* GetVariable(const std::string& name) const;

	void AddLabel(const std::string& name, uint32_t offset);
	uint32_t GetLabel(const std::string& name);

	void AddFunctionDeclaration(const Function& func);
	const BlockContext::Function* const GetFunction(const std::string& name) const;
	

private:
	std::map<std::string, uint32_t> _offsets;

	std::map<std::string, Type> _types;
	std::map<std::string, Function> _functions;
	std::map<std::string, ContextVariable> _variables;

	std::vector<BlockContext> _subContexts;
	BlockContext* _parentContext;

	uint32_t _variableCount;
};
