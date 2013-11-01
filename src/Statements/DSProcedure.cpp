#include "Statements/DSProcedure.hpp"

namespace ds
{

DSProcedure::DSProcedure
(
	const std::vector<std::string>& paremterNames,
	std::unique_ptr<const ParameterList> parameters,
	std::unique_ptr<const Statement> body
)
	:parameterNames(parameterNames)
	,parameters(parameters.release())
	,body(body.release())
{
}

void DSProcedure::operator()(Context& context) const
{
	context.pushStack();
	for (int i = 0; i < parameters->size(); ++i)
	{
		switch (parameters->getTypeOfParameter(i))
		{
			case ParameterList::Type::Number:
				context.setFlag("local", parameterNames[i], parameters->getNumericalParameter(context, i));
				break;
			case ParameterList::Type::String:
				context.setString("local", parameterNames[i], parameters->getStringParameter(context, i));
				break;
		}
	}
	
	body->execute(context);
	
	context.popStack();
}

}