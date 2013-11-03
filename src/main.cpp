#include <iostream>
#include "SyntaxErrorException.hpp"
#include "RuntimeInterpreterErrorException.hpp"
#include "Interpreter.hpp"

int main(int argc, char **argv)
{
	ds::Interpreter interpreter;
	if (argc == 1)
	{
		interpreter.use();
	}
	else
	{
		try
		{
			interpreter.run(argv[1]);
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
