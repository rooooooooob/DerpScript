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

void println(Context& context, const ParameterList& params)
{
	std::cout << params.getStringParameter(context, 0) << std::endl;
}

void printlnNum(Context& context, const ParameterList& params)
{
	std::cout << params.getNumericalParameter(context, 0) << std::endl;
}

std::string read(Context& context, const ParameterList& params)
{
	std::string ret;
	std::cin >> ret;
	return ret;
}

std::string readln(Context& context, const ParameterList& params)
{
	std::string ret;
	std::getline(std::cin, ret);
	return ret;
}

}