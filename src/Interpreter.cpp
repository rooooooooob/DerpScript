#include "Interpreter.hpp"

#include "FileParser.hpp"
#include "ParameterList.hpp"

namespace ds
{

Interpreter::Interpreter()
	:context()
{
}

Interpreter::~Interpreter()
{
}

void Interpreter::interpret(const char *fname)
{
	loadFile(context, fname);
	ParameterList params;
	context.executeProcedure("local", "main", params);
}

}