#include "Lib/StringLib.hpp"

#include <sstream>
#include "Context.hpp"
#include "ParameterList.hpp"

namespace ds
{

std::string str(Context& context, const ParameterList& params)
{
	std::stringstream ss;
	ss << params.getNumericalParameter(context, 0);
	return ss.str();
}

float strlen(Context& context, const ParameterList& params)
{
	return params.getStringParameter(context, 0).size();
}

float num(Context& context, const ParameterList& params)
{
	return std::stof(params.getStringParameter(context, 0));
}

}