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
	if (!types.empty() && types.back() == StringLiteral)
	{
		literals.back() += stringLiteral;
	}
	else
	{
		functions.push_back(nullptr);
		literals.push_back(stringLiteral);
		types.push_back(StringLiteral);
	}
}

void StringConcatenation::addStringFunction(const Function *function)
{
	functions.push_back(function);
	literals.push_back("invalid");
	types.push_back(StringFunction);
}

std::string StringConcatenation::concatenate(const Context& context) const
{
	std::string buffer;
	for (std::size_t i = 0; i < types.size(); ++i)
	{
		if (types[i] == StringLiteral)
		{
			buffer += literals[i];
		}
		else
		{
			buffer += functions[i]->evaluateAsString(context);
		}
	}
	return buffer;
}

}
