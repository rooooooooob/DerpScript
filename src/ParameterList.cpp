#include "ParameterList.hpp"

#include <cassert>
#include "Expressions/Expression.hpp"
#include "Expressions/Function.hpp"
#include "Context.hpp"

namespace ds
{

ParameterList::ParameterList()
	:strings()
	,numericalExpressions()
	,stringFunctions()
	,types()
	,signature("")
{
}

void ParameterList::addStringParameter(const std::string& stringParam)
{
	strings.push_back(stringParam);
	numericalExpressions.push_back(nullptr);
	stringFunctions.push_back(nullptr);
	types.push_back(InternalType::StringLiteral);
	signature += 'S';
}

void ParameterList::addStringParameter(std::unique_ptr<const StringConcatenation> stringConcat)
{
	strings.push_back("invalid parameter");
	numericalExpressions.push_back(nullptr);
	stringFunctions.push_back(std::move(stringConcat));
	types.push_back(InternalType::StringExpression);
	signature += "S";
}

void ParameterList::addNumericalParameter(const Expression *expParam)
{
	strings.push_back("invalid parameter");
	numericalExpressions.push_back(expParam);
	stringFunctions.push_back(nullptr);
	types.push_back(InternalType::NumericalExpression);
	signature += 'N';
}

const std::string& ParameterList::getSignature() const
{
	return signature;
}

std::string ParameterList::getFormattedSignature() const
{
	std::string buffer("(");
	for (std::size_t i = 0; i < types.size(); ++i)
	{
		if (i > 0)
		{
			buffer += ", ";
		}
		if (types[i] == InternalType::StringLiteral || types[i] == InternalType::StringExpression)
		{
			buffer += "String";
		}
		else if (types[i] == InternalType::NumericalExpression)
		{
			buffer += "Number";
		}
	}
	buffer += ")";
	return buffer;
}

float ParameterList::getNumericalParameter(Context& context, int parameterIndex) const
{
	assert(parameterIndex >= 0 && parameterIndex < types.size());
	assert(types[parameterIndex] == InternalType::NumericalExpression);
	assert(strings.size() == numericalExpressions.size() &&
		   numericalExpressions.size() == stringFunctions.size() &&
		   stringFunctions.size() == types.size());
	return numericalExpressions[parameterIndex]->evaluate(context);
}

std::string ParameterList::getStringParameter(Context& context, int parameterIndex) const
{
	assert(parameterIndex >= 0 && parameterIndex < types.size());
	assert(types[parameterIndex] == InternalType::StringLiteral ||
	       types[parameterIndex] == InternalType::StringExpression);
	assert(strings.size() == numericalExpressions.size() &&
		   numericalExpressions.size() == stringFunctions.size() &&
		   stringFunctions.size() == types.size());
	if (types[parameterIndex] == InternalType::StringLiteral)
	{
		return strings[parameterIndex];
	}
	else
	{
		return stringFunctions[parameterIndex]->concatenate(context);
	}
}

ParameterList::Type ParameterList::getTypeOfParameter(int parameterIndex) const
{
	assert(parameterIndex >= 0 && parameterIndex < types.size());
	switch (types[parameterIndex])
	{
		case InternalType::StringLiteral:
			return Type::String;
		case InternalType::StringExpression:
			return Type::String;
		case InternalType::NumericalExpression:
			return Type::Number;
	}
	return Type::Number;//stop dumb warnings
}

int ParameterList::size() const
{
	return strings.size();
}

}
