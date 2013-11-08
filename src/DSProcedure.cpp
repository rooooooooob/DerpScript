#include "DSProcedure.hpp"

#include "RuntimeInterpreterErrorException.hpp"
#include "StackPush.hpp"

namespace ds
{

DSProcedure::DSProcedure(const std::vector<std::string>& parameterNames,
	const std::vector<ParameterList::Type>& parameterTypes,
	std::unique_ptr<const Statement> body
)
	:parameterNames(parameterNames)
	,parameterTypes(parameterTypes)
	,body(body.release())
{
}

DSProcedure::DSProcedure(DSProcedure& other)
	:parameterNames(other.parameterNames)
	,parameterTypes(other.parameterTypes)
	,body(other.body.release())
{
}

DSProcedure& DSProcedure::operator=(DSProcedure& other)
{
	parameterNames = other.parameterNames;
	parameterTypes = other.parameterTypes;
	body.reset(other.body.release());
	return *this;
}


void DSProcedure::operator()(Context& context, const ParameterList& parameters) const
{
	StackPush push(context, parameters, parameterNames, parameterTypes);

	body->execute(context);
}

}
