#include "Lib/IOLib.hpp"

#include <iostream>
#include "Context.hpp"
#include "ParameterList.hpp"

namespace ds
{

void print(Context& context, const ParameterList& params)
{
	std::cout << params.getStringParameter(context, 0);
}

void printNum(Context& context, const ParameterList& params)
{
	std::cout << params.getNumericalParameter(context, 0);
}

}