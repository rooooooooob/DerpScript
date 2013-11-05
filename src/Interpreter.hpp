#ifndef DS_INTERPRETER_HPP
#define DS_INTERPRETER_HPP

#include <string>
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

	template <typename F>
	void registerLibraryFunction(const std::string& scope, const std::string& name, const std::string& paramTypes, F function);

	template <typename F>
	void registerLibraryStringFunction(const std::string& scope, const std::string& name, const std::string& paramTypes, F function);
	
	template <typename F>
	void registerLibraryProcedure(const std::string& scope, const std::string& name, const std::string& paramTypes, F function);

	Context context;
};

template <typename F>
void Interpreter::registerLibraryFunction(const std::string& scope, const std::string& name, const std::string& paramTypes, F function)
{
	Context& context(context);
	context.registerFunction(scope, name, paramTypes, [&context, function](const ParameterList& params) -> float
	{
		return function(context, params);
	});
}

template <typename F>
void Interpreter::registerLibraryStringFunction(const std::string& scope, const std::string& name, const std::string& paramTypes, F function)
{
	Context& context(context);
	context.registerStringFunction(scope, name, paramTypes, [&context, function](const ParameterList& params) -> std::string
	{
		return function(context, params);
	});
}

template <typename F>
void Interpreter::registerLibraryProcedure(const std::string& scope, const std::string& name, const std::string& paramTypes, F function)
{
	Context& context(context);
	context.registerProcedure(scope, name, paramTypes, [&context, function](const ParameterList& params)
	{
		function(context, params);
	});
}

}

#endif