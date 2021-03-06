#ifndef DS_PARAMETERLIST_HPP
#define DS_PARAMETERLIST_HPP

#include <string>
#include <vector>
#include <memory>
#include "StringConcatenation.hpp"

namespace ds
{

class Expression;

class Context;

class Function;

class ParameterList
{
public:
	enum class Type
	{
		Number,
		String
	};


	ParameterList();

	void addStringParameter(const std::string& stringParam);

	void addStringParameter(std::unique_ptr<const StringConcatenation> stringConcat);

	void addNumericalParameter(const Expression *expParam);

	const std::string& getSignature() const;

	std::string getFormattedSignature() const;

	float getNumericalParameter(Context& context, int parameterIndex) const;

	std::string getStringParameter(Context& context, int parameterIndex) const;

	Type getTypeOfParameter(int parameterIndex) const;

	int size() const;

private:
	enum class InternalType
	{
		StringLiteral,
		StringExpression,
		NumericalExpression
	};

	std::vector<std::string> strings;
	std::vector<const Expression*> numericalExpressions;
	std::vector<std::unique_ptr<const StringConcatenation> > stringFunctions;
	std::vector<InternalType> types;
	std::string signature;
};

}

#endif // DS_PARAMETERLIST_HPP
