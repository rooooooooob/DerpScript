#include <iostream>
#include "ParserTester.hpp"
#include "SyntaxErrorException.hpp"
#include "RuntimeInterpreterErrorException.hpp"
#include "Interpreter.hpp"

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		ds::useParser();
	}
	else
	{
		ds::Interpreter interpreter;
		try
		{
			interpreter.interpret(argv[1]);
		}
		catch (ds::SyntaxErrorException& see)
		{
			std::cerr << "Syntax error:\n" << see.what();
			return 1;
		}
		catch (ds::RuntimeInterpreterErrorException& riee)
		{
			std::cerr << "Runtime error:\n" << riee.what();
		}
	}
	return 0;
}
