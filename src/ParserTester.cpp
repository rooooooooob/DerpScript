#include "ParserTester.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "Parser.hpp"
#include "ExpressionParser.hpp"
#include "FileParser.hpp"
#include "Expressions/Constant.hpp"
#include "SyntaxErrorException.hpp"
#include "RuntimeInterpreterErrorException.hpp"

/*
	TODO:	 fix a ++ b crashing
*/

namespace ds
{

void useParser()
{
	std::cout << "Welcome to the parsing tester!" << std::endl;
	std::string input;
	Context context;
	context.registerFunction("local", "foo", "NN", std::function<float(const ParameterList&)>(
		[&context](const ParameterList& params) -> float
		{
			int a = (int) params.getNumericalParameter(context, 0);
			int b = (int) params.getNumericalParameter(context, 1);
			return a + b;
		}
	));
	context.registerFunction("local", "fib", "N", std::function<float(const ParameterList&)>(
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
	context.registerFunction("local", "length", "S", std::function<float(const ParameterList&)>(
		[&context](const ParameterList& params) -> float
		{
			std::string input = params.getStringParameter(context, 0);
			return input.size();
		}
	));
	context.registerFunction("local", "print", "S", std::function<float(const ParameterList&)>(
		[&context](const ParameterList& params) -> float
		{
			std::string input = params.getStringParameter(context, 0);
			std::cout << "echo: " << input << std::endl;
			return 0.f;
		}
	));
	context.registerProcedure("local", "print", "S", std::function<void(const ParameterList&)>(
		[&context](const ParameterList& params) -> void
		{
			std::string input = params.getStringParameter(context, 0);
			std::cout << "echo: " << input << std::endl;
		}
	));
	context.registerFunction("local", "multiPrint", "SN", std::function<float(const ParameterList&)>(
		[&context](const ParameterList& params) -> float
		{
			std::string input = params.getStringParameter(context, 0);
			int n = (int) params.getNumericalParameter(context, 1);
			for (int i = 0; i < n; ++i)
				std::cout << "echo: " << input << std::endl;
			return 0.f;
		}
	));
	context.registerStringFunction("local", "repeat", "SN", std::function<std::string(const ParameterList&)>(
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
			std::cout << ">";
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
			}
			else if (input.compare(0, 4, "load") == 0)
			{
				loadFile(context, input.c_str() + 4);
				std::cout << "Loaded " << input.c_str() + 4 << std::endl;
			}
			else if (input.compare(0, 4, "exit") == 0)
			{
				break;
			}
			else
			{
				std::cout << "Invalid input. Try: 'eval <expression>', 'exec', 'run <filename>', or 'exit'" << std::endl;
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

}
