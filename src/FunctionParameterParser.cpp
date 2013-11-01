#include "FunctionParameterParser.hpp"

#include <sstream>
#include "ExpressionParser.hpp"
#include "StringParser.hpp"
#include "SyntaxErrorException.hpp"

namespace ds
{

static void addParameter(ParameterList& params, const std::string& buffer);

std::unique_ptr<const ParameterList> parseFunctionParameters(const char *c)
{
	int depthLevel = 0;
	const char *startOfParam = c;
	std::unique_ptr<ParameterList> params(new ParameterList());
	while (*c != '\0')
	{
		if (*c == '(')
		{
			++depthLevel;
		}
		else if (*c == ')')
		{
			if (--depthLevel < 0)
			{
				std::stringstream ss;
				ss << "Unbalanced parenthesis in function parameters '"
				   << c
				   << "' - too many )s";
				throw SyntaxErrorException(ss.str());
			}
		}
		else if (*c == ',' && depthLevel == 0)	//	move onto the next param
		{
			std::string buffer(startOfParam, c - startOfParam);
			//std::cout << "!arg is {" << buffer << "}" << std::endl;
			addParameter(*params, buffer);
			startOfParam = c + 1;
		}
		++c;
	}
	if (depthLevel > 0)
	{
		std::stringstream ss;
		ss << "Unbalanced parenthesis in function parameters '"
			<< c
			<< "' - too many (s";
		throw SyntaxErrorException(ss.str());
	}
	std::string buffer(startOfParam,  c - startOfParam);
	//std::cout << "~arg is {" << buffer << "}" << std::endl;
	if (!buffer.empty())
	{
		addParameter(*params, buffer);
	}
	//std::cout << "paramlist created: " << params->getFormattedSignature() << std::endl;
	return std::unique_ptr<const ParameterList>(params.release());
}

void addParameter(ParameterList& params, const std::string& buffer)
{
	if (buffer.empty())
	{

	}
	if (buffer.size() > 1 && (buffer.back() == '"' || buffer.back() == ']'))	//	We're dealing with strings
	{
		params.addStringParameter(parseStringExpression(buffer.c_str()));
	}
	else	//	Nope, just normal numerical expressions
	{
		params.addNumericalParameter(parseExpression(buffer.c_str()));
	}
}

}
