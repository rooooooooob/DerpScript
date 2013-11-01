#include "Statements/SetVariable.hpp"

namespace ds
{

SetVariable::SetVariable(const std::string& scope, const std::string& name, const Expression *expression)
	:scope(scope)
	,name(name)
	,expression(expression)
{
}

void SetVariable::execute(Context& context) const
{
	context.setVar(scope, name, expression->evaluate(context));
}

}
