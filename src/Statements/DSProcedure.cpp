#include "DSProcedure.hpp"

#include "RuntimeInterpreterErrorException.hpp"

namespace ds
{

DSProcedure::DSProcedure(Context& context,
	const std::vector<std::string>& parameterNames,
	const std::vector<std::string>& parameterTypes,
	std::unique_ptr<const Statement> body
)
	:context(context)
	,parameterNames(parameterNames)
	,parameterTypes(parameterTypes)
	,body(body.release())
{
}

void DSProcedure::operator()(const ParameterList& parameters) const
{
	context.pushStack();
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
				context.setFlag("local", parameterNames[i], parameters.getNumericalParameter(context, i));
				break;
			case ParameterList::Type::String:
				context.setString("local", parameterNames[i], parameters.getStringParameter(context, i));
				break;
		}
	}
	
	body->execute(context);
	
	context.popStack();
}

}