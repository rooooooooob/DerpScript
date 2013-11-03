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

	void run(const char *fname);

	void use();
	
private:
	void loadSTDLibs();

	Context context;
};

}

#endif