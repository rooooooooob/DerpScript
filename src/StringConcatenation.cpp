#include "StringConcatenation.hpp"

#include "Expressions/Function.hpp"
#include "Context.hpp"

namespace ds
{

StringConcatenation::StringConcatenation()
{
}

StringConcatenation::~StringConcatenation()
{
	for (const Function *f : functions)
	{
		if (f)
		{
			delete f;
		}
	}
}

void StringConcatenation::addStringLiteral(const std::string& stringLiteral)
{
	if (!types.empty() && types.back() == Type::StringLiteral)
	{
		literals.back() += stringLiteral;
	}
	else
	{
		functions.push_back(nullptr);
		literals.push_back(stringLiteral);
		scopes.push_back("");
		types.push_back(Type::StringLiteral);
	}
}

void StringConcatenation::addStringFunction(const Function *function)
{
	functions.push_back(function);
	literals.push_back("invalid");
	scopes.push_back("");
	types.push_back(Type::StringFunction);
}

void StringConcatenation::addStringVariable(const std::string& scope, const std::string& name)
{
	functions.push_back(nullptr);
	literals.push_back(name);
	scopes.push_back(scope);
	types.push_back(Type::StringVariable);
}

void StringConcatenation::addStringFlag(const std::string& scope, const std::string& name)
{
	functions.push_back(nullptr);
	literals.push_back(name);
	scopes.push_back(scope);
	types.push_back(Type::StringFlag);
}

std::string StringConcatenation::concatenate(const Context& context) const
{
	std::string buffer;
	for (std::size_t i = 0; i < types.size(); ++i)
	{
		switch (types[i])
		{
			case Type::StringLiteral:
				buffer += literals[i];
				break;
			case Type::StringFunction:
				buffer += functions[i]->evaluateAsString(context);
				break;
			case Type::StringVariable:
				buffer += context.getString(scopes[i], literals[i]);
				break;
			case Type::StringFlag:
				buffer += context.getStringFlag(scopes[i], literals[i]);
				break;
		}
	}
	return buffer;
}

}
