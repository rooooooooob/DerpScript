#ifndef DS_INTERPRETER_HPP
#define DS_INTERPRETER_HPP

#include "Context.hpp"

namespace ds
{

class Interpreter
{
public:
	Interpreter();
	~Interpreter();

	void interpret(const char *fname);

private:
	Context context;
};

}

#endif