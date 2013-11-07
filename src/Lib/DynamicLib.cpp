#include "Lib/DynamicLib.hpp"

#include "Context.hpp"
#include "ParameterList.hpp"
#include "ExpressionParser.hpp"
#include "Parser.hpp"
#include "FileParser.hpp"

namespace ds
{

float eval(Context& context, const ParameterList& params)
{
	std::string arg(params.getStringParameter(context, 0));
	std::unique_ptr<const Expression> expression(parseExpression(arg.c_str()));
	return expression->evaluate(context);
}

void exec(Context& context, const ParameterList& params)
{
	std::string arg(params.getStringParameter(context, 0));
	std::vector<std::string> strings;
	strings.push_back(arg);
	executeStrings(strings, context);
}

void load(Context& context, const ParameterList& params)
{
	std::string fname(params.getStringParameter(context, 0));
	loadFile(context, fname.c_str());
}

}