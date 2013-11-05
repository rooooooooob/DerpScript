#include "Lib/IOLib.hpp"

#include <iostream>
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

std::string fileread(Context& context, const ParameterList& params)
{
	std::string ret;
	std::string fname = params.getStringParameter(context, 0);
	std::fstream& file = context.getFile(fname);
	if (file.good())
	{
		file >> ret;
	}
	else
	{
		ret = "<error reading from " + fname + ">";
	}
	return ret;
}

std::string filereadln(Context& context, const ParameterList& params)
{
	std::string ret;
	std::string fname = params.getStringParameter(context, 0);
	std::fstream& file = context.getFile(fname);
	if (file.good())
	{
		std::getline(file, ret);
	}
	else
	{
		ret = "<error reading from " + fname + ">";
	}
	return ret;
}

void filewrite(Context& context, const ParameterList& params)
{
	std::string fname = params.getStringParameter(context, 0);
	std::fstream& file = context.getFile(fname);
	file << params.getStringParameter(context, 1);
}

void open(Context& context, const ParameterList& params)
{
	context.openFile(params.getStringParameter(context, 0));
}

void close(Context& context, const ParameterList& params)
{
	context.closeFile(params.getStringParameter(context, 0));
}

}