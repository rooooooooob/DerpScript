#include "StackPush.hpp"

#include "RuntimeInterpreterErrorException.hpp"
#include "Context.hpp"

namespace ds
{

StackPush::StackPush(Context& context, const ParameterList& parameters, const std::vector<std::string>& parameterNames,
		const std::vector<ParameterList::Type>& parameterTypes)
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
}

}