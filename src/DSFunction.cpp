#include "DSFunction.hpp"

#include "RuntimeInterpreterErrorException.hpp"
#include "StackPush.hpp"

namespace ds
{

DSFunction::DSFunction(const std::vector<std::string>& parameterNames,
	const std::vector<ParameterList::Type>& parameterTypes,
	std::unique_ptr<const Statement> body
)
	:parameterNames(parameterNames)
	,parameterTypes(parameterTypes)
	,body(body.release())
{
}

DSFunction::DSFunction(DSFunction& other)
	:parameterNames(other.parameterNames)
	,parameterTypes(other.parameterTypes)
	,body(other.body.release())
{
}

DSFunction& DSFunction::operator=(DSFunction& other)
{
	parameterNames = other.parameterNames;
	parameterTypes = other.parameterTypes;
	body.reset(other.body.release());
	return *this;
}


float DSFunction::operator()(Context& context, const ParameterList& parameters) const
{
	StackPush push(context, parameters, parameterNames, parameterTypes);

	body->execute(context);

	return context.getReturnValue();
}

}
