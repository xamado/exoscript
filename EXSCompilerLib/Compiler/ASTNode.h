#pragma once

#include "Compiler/ILGenerator.h"
#include "Compiler/BlockContext.h"

#include "IL/ILInstructionSet.h"

#include <stdint.h>
#include <vector>

class ASTNode
{
public:
	virtual ~ASTNode() {}

	virtual bool GenerateCode(BlockContext& context, ILByteCode* bytecode) const = 0;
};

class ASTExpressionNode : public ASTNode
{
public:
	virtual ~ASTExpressionNode() {}

	virtual BlockContext::Type GetType(const BlockContext& context) const = 0;
};

class ASTStatementNode : public ASTNode
{
public:
	virtual ~ASTStatementNode() {}
};

class ASTExpressionStatementNode : public ASTStatementNode
{
public:
	ASTExpressionStatementNode(const ASTExpressionNode& expr)
		: _expr(expr)
	{
		
	}

	bool GenerateCode(BlockContext& context, ILByteCode* bytecode) const override;

private:
	const ASTExpressionNode& _expr;
};

class ASTIdentifierNode : public ASTExpressionNode
{
public:
	ASTIdentifierNode(const std::string& name) 
		: _name(name)
	{
		
	}

	virtual ~ASTIdentifierNode() {}

	const std::string& GetName() const { return _name; }

	bool GenerateCode(BlockContext& context, ILByteCode* bytecode) const override;
	
	BlockContext::Type GetType(const BlockContext& context) const override 
	{ 
		const BlockContext::ContextVariable* var = context.GetVariable(_name);
		if(var == nullptr)
		{
			return BlockContext::Type::Invalid;
		}

		return var->GetType();
	};

private:
	const std::string& _name;
};




class ASTBlockNode : public ASTNode
{
public:
	virtual ~ASTBlockNode() {}

	std::vector<const ASTStatementNode*>& GetStatements() { return _statements; }

	bool GenerateCode(BlockContext& context, ILByteCode* bytecode) const override;

private:
	std::vector<const ASTStatementNode*> _statements;
};




class ASTVariableDeclarationNode : public ASTStatementNode
{
public:
	ASTVariableDeclarationNode(const ASTIdentifierNode& type, const ASTIdentifierNode& id)
		: _type(type), _id(id), _rhs(nullptr)
	{
	}

	ASTVariableDeclarationNode(const ASTIdentifierNode& type, const ASTIdentifierNode& id, const ASTExpressionNode& expression)
		: _type(type), _id(id), _rhs(&expression)
	{
	}

	const ASTIdentifierNode& GetTypeIdentifier() const
	{
		return _type;
	}

	const ASTIdentifierNode& GetNameIdentifier() const
	{
		return _id;
	}

	bool GenerateCode(BlockContext& context, ILByteCode* bytecode) const override;

private:
	const ASTIdentifierNode& _type;
	const ASTIdentifierNode& _id;
	
	const ASTExpressionNode* _rhs;
};




class ASTFunctionParametersNode : public ASTNode
{
public:
	ASTFunctionParametersNode()
	{
		
	}

	void Add(ASTVariableDeclarationNode& var)
	{
		_variables.push_back(&var);
	}

	std::vector<const ASTVariableDeclarationNode*> GetParameters() const
	{
		return _variables;
	}

	bool GenerateCode(BlockContext& context, ILByteCode* bytecode) const override;

private:
	std::vector<const ASTVariableDeclarationNode*> _variables;
};





class ASTFunctionArgumentsNode : public ASTNode
{
public:
	ASTFunctionArgumentsNode()
	{

	}

	void Add(ASTExpressionNode& arg)
	{
		_arguments.push_back(&arg);
	}

	uint32_t GetArgumentCount() const { return _arguments.size(); }

	bool GenerateCode(BlockContext& context, ILByteCode* bytecode) const override;

private:
	std::vector<const ASTExpressionNode*> _arguments;
};




class ASTFunctionDeclarationNode : public ASTStatementNode
{
public:
	ASTFunctionDeclarationNode(const ASTIdentifierNode& ret, const ASTIdentifierNode& name, const ASTFunctionParametersNode& params, const ASTBlockNode& block)
		: _name(name.GetName()), _block(block), _parameters(params)
	{
		
	}

	bool GenerateCode(BlockContext& context, ILByteCode* bytecode) const override;

private:
	const std::string& _name;
	const ASTBlockNode& _block;
	const ASTFunctionParametersNode& _parameters;
};




class ASTFunctionCallNode : public ASTExpressionNode
{
public:
	ASTFunctionCallNode(const ASTIdentifierNode& id, const ASTFunctionArgumentsNode& arguments)
		: _identifier(id), _arguments(arguments)
	{
		
	}

	bool GenerateCode(BlockContext& context, ILByteCode* bytecode) const override;

	BlockContext::Type GetType(const BlockContext& context) const override
	{ 
		// TODO: Functions dont have a return type yet :)
		return BlockContext::Type::Void;
	};

private:
	const ASTIdentifierNode& _identifier;
	const ASTFunctionArgumentsNode& _arguments;
};





class ASTAssignmentNode : public ASTStatementNode
{
public:
	ASTAssignmentNode(const ASTIdentifierNode& lhs, const ASTExpressionNode& rhs)
		: _lhs(lhs), _rhs(rhs)
	{
		
	}

	bool GenerateCode(BlockContext& context, ILByteCode* bytecode) const override;

private:
	const ASTIdentifierNode& _lhs;
	const ASTExpressionNode& _rhs;
};




class ASTBinaryOperatorNode : public ASTExpressionNode
{
public:
	ASTBinaryOperatorNode(const ASTExpressionNode& lhs, uint8_t op, const ASTExpressionNode& rhs) :
		op(op), lhs(lhs), rhs(rhs) 
	{ 

	}
	
	virtual ~ASTBinaryOperatorNode() {	}

	bool GenerateCode(BlockContext& context, ILByteCode* bytecode) const override;

	BlockContext::Type GetType(const BlockContext& context) const override 
	{ 
		// TODO: Should lhs and rhs match? should we check that here?
		return lhs.GetType(context); 
	};

private:
	uint32_t op;
	const ASTExpressionNode& lhs;
	const ASTExpressionNode& rhs;	
};




class ASTIntegerNode : public ASTExpressionNode
{
public:
	ASTIntegerNode(uint32_t value) : _value(value)
	{

	}

	virtual ~ASTIntegerNode() {}

	bool GenerateCode(BlockContext& context, ILByteCode* bytecode) const override;

	BlockContext::Type GetType(const BlockContext& context) const override 
	{ 
		return BlockContext::Type::Integer; 
	}

private:
	uint32_t _value;
};




class ASTDoubleNode : public ASTExpressionNode
{
public:
	ASTDoubleNode(double value) : _value(value)
	{

	}

	virtual ~ASTDoubleNode() {}

	bool GenerateCode(BlockContext& context, ILByteCode* bytecode) const override;

	BlockContext::Type GetType(const BlockContext& context) const override 
	{ 
		return BlockContext::Type::Float; 
	};

private:
	double _value;
};




class ASTPrintNode : public ASTStatementNode
{
public:
	ASTPrintNode(const ASTExpressionNode& expr) 
		: _expression(expr)
	{
		
	}

	bool GenerateCode(BlockContext& context, ILByteCode* bytecode) const override;

private:
	const ASTExpressionNode& _expression;
};