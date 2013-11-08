#include "DSFunction.hpp"

#include "RuntimeInterpreterErrorException.hpp"

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
	for (int i = 0; i < parameters.size(); ++i)
	{
		ParameterList::Type type = parameters.getTypeOfParameter(i);
		if (type != parameterTypes[i])
		{
			context.popStack();

			throw RuntimeInterpreterErrorException("Type mismatch on function call somewhere.");
		}
		switch (type)
		{
			case ParameterList::Type::Number:
				context.pushStackNumber(parameterNames[i], parameters.getNumericalParameter(context, i));
				break;
			case ParameterList::Type::String:
				context.pushStackString(parameterNames[i], parameters.getStringParameter(context, i));
				break;
		}
	}

	context.pushStack();

	body->execute(context);

	context.popStack();

	return context.getReturnValue();
}

}
