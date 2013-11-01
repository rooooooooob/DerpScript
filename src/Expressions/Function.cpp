#include "Expressions/Function.hpp"

#include "ParameterList.hpp"

namespace ds
{

Function::Function(const std::string& scope, const std::string& name, std::unique_ptr<const ParameterList> parameters)
	:scope(scope)
	,name(name)
	,parameters(parameters.release())
{
}

float Function::evaluate(const Context& context) const
{
	return context.evaluateNumericalFunction(scope, name, *parameters);
}

std::string Function::evaluateAsString(const Context& context) const
{
	return context.evaluateStringFunction(scope, name, *parameters);
}

}
