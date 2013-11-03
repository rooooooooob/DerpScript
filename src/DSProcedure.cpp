#include "DSProcedure.hpp"

#include "RuntimeInterpreterErrorException.hpp"

namespace ds
{

DSProcedure::DSProcedure(Context& context,
	const std::vector<std::string>& parameterNames,
	const std::vector<ParameterList::Type>& parameterTypes,
	std::unique_ptr<const Statement> body
)
	:context(&context)
	,parameterNames(parameterNames)
	,parameterTypes(parameterTypes)
	,body(body.release())
{
}

DSProcedure::DSProcedure(DSProcedure& other)
	:context(other.context)
	,parameterNames(other.parameterNames)
	,parameterTypes(other.parameterTypes)
	,body(other.body.release())
{
}

DSProcedure& DSProcedure::operator=(DSProcedure& other)
{
	context = other.context;
	parameterNames = other.parameterNames;
	parameterTypes = other.parameterTypes;
	body.reset(other.body.release());
	return *this;
}


void DSProcedure::operator()(const ParameterList& parameters) const
{
	for (int i = 0; i < parameters.size(); ++i)
	{
		ParameterList::Type type = parameters.getTypeOfParameter(i);
		if (type != parameterTypes[i])
		{
			context->popStack();

			throw RuntimeInterpreterErrorException("Type mismatch on function call somewhere.");
		}
		switch (type)
		{
			case ParameterList::Type::Number:
				context->pushStackNumber(parameterNames[i], parameters.getNumericalParameter(*context, i));
				break;
			case ParameterList::Type::String:
				context->pushStackString(parameterNames[i], parameters.getStringParameter(*context, i));
				break;
		}
	}

	context->pushStack();
	
	body->execute(*context);
	
	context->popStack();
}

}