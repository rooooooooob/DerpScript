#include "Interpreter.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <functional>

#include "Parser.hpp"
#include "ExpressionParser.hpp"
#include "FileParser.hpp"
#include "Expressions/Constant.hpp"
#include "SyntaxErrorException.hpp"
#include "RuntimeInterpreterErrorException.hpp"
#include "ParserUtils.hpp"
#include "ParameterList.hpp"
#include "Lib/StringLib.hpp"
#include "Lib/IOLib.hpp"
#include "Lib/MathLib.hpp"

/*
	TODO:	 fix a ++ b crashing
*/

namespace ds
{

Interpreter::Interpreter()
	:context()
{
	this->loadSTDLibs();
}

Interpreter::~Interpreter()
{
}

void Interpreter::run(const char *fname)
{
	loadFile(context, fname);
	ParameterList params;
	context.executeProcedure("local", "main", params);
}

void Interpreter::use()
{
	std::cout << "Welcome to the parsing tester!" << std::endl;
	std::string input;
	Context& context(context);
	context.registerFunction("tester", "fib", "N", std::function<float(const ParameterList&)>(
		[&context](const ParameterList& params) -> float
		{
			int n = (int) params.getNumericalParameter(context, 0);
			ParameterList p1, p2;
			p1.addNumericalParameter(new Constant(n - 1.f));
			p2.addNumericalParameter(new Constant(n - 2.f));
			if (n > 1)
				return context.evaluateNumericalFunction("local", "fib", p1) + context.evaluateNumericalFunction("local", "fib", p2);
			else
				return 1.f;
		}
	));
	context.registerFunction("tester", "multiPrint", "SN", std::function<float(const ParameterList&)>(
		[&context](const ParameterList& params) -> float
		{
			std::string input = params.getStringParameter(context, 0);
			int n = (int) params.getNumericalParameter(context, 1);
			for (int i = 0; i < n; ++i)
				std::cout << "echo: " << input << std::endl;
			return 0.f;
		}
	));
	context.registerStringFunction("tester", "repeat", "SN", std::function<std::string(const ParameterList&)>(
		[&context](const ParameterList& params) -> std::string
		{
			std::string input = params.getStringParameter(context, 0);
			int n = (int) params.getNumericalParameter(context, 1);
			std::stringstream ss;
			for (int i = 0; i < n; ++i)
				ss << input;
			return ss.str();
		}
	));
	for ( ; ; )
	{
		try
		{
			std::cout << "\n>";
			std::getline(std::cin, input);
			if (input.compare(0, 4, "eval") == 0)
			{
				Expression *exp = parseExpression(input.c_str() + 4);
				std::cout << exp->evaluate(context) << std::endl;
				delete exp;
			}
			else if (input.compare(0, 4, "exec") == 0)
			{
				std::vector<std::string> lines;
				std::cout << "Enter lines to get executed. Type \"end\" to finish." << std::endl;
				for(int line = 1; ; ++line)
				{
					for (int i = 1000; i > line; i /= 10)
					{
						std::cout << " ";
					}
					std::cout << line << "|";
					std::getline(std::cin, input);
					if (input == "end")
					{
						executeStrings(lines, context);
						break;
					}
					else
					{
						lines.push_back(input);
					}
				}
			}
			else if (input.compare(0, 3, "run") == 0)
			{
				const char *fname = input.c_str() + 4;
				eatWhitespace(fname);
				loadFile(context, fname);
				std::cout << "Running " << fname << "..." << std::endl;
				ParameterList params;
				context.executeProcedure("local", "main", params);
				std::cout << "Finished running " << fname << std::endl;
			}
			else if (input.compare(0, 4, "load") == 0)
			{
				const char *fname = input.c_str() + 4;
				eatWhitespace(fname);
				loadFile(context, fname);
				std::cout << "Loaded " << fname << std::endl;
			}
			else if (input.compare(0, 4, "exit") == 0)
			{
				break;
			}
			else
			{
				std::cout << "Invalid input.\nTry: 'eval <expression>', 'exec', 'load <file>', 'run <file>', or 'exit'" << std::endl;
			}
		}
		catch (SyntaxErrorException& see)
		{
			std::cout << "Syntax Error Occured:\n" << see.what() << std::endl;
		}
		catch (RuntimeInterpreterErrorException& riee)
		{
			std::cout << "Runtime Error Occured:\n" << riee.what() << std::endl;
		}
	}
}

void Interpreter::loadSTDLibs()
{
	//	StringLib
	registerLibraryStringFunction("local", "str", "N", str);
	registerLibraryFunction("local", "strlen", "S", strlen);
	registerLibraryFunction("local", "num", "S", num);
	//	IOLib
	registerLibraryProcedure("local", "print", "S", print);
	registerLibraryProcedure("local", "print", "N", printNum);
	registerLibraryProcedure("local", "println", "S", println);
	registerLibraryProcedure("local", "println", "N", printlnNum);
	registerLibraryStringFunction("local", "read", "", read);
	registerLibraryStringFunction("local", "readln", "", readln);
	//	MathLib
	registerLibraryFunction("local", "sqrt", "N", dssqrt);
	registerLibraryFunction("local", "ceil", "N", dsceil);
	registerLibraryFunction("local", "floor", "N", dsfloor);
	registerLibraryFunction("local", "round", "N", dsround);
	registerLibraryFunction("local", "pow", "NN", dspow);
	registerLibraryFunction("local", "sin", "N", dssin);
	registerLibraryFunction("local", "cos", "N", dscos);

}

}