#include "Statements/SetFlag.hpp"

namespace ds
{

SetFlag::SetFlag(const std::string& scope, const std::string& name, const Expression *expression)
	:scope(scope)
	,name(name)
	,expression(expression)
{
}

void SetFlag::execute(Context& context) const
{
	context.setFlag(scope, name, expression->evaluate(context));
}

}

