#include "Statements/ReturnStatement.hpp"

namespace ds
{

ReturnStatement::ReturnStatement(std::unique_ptr<const Expression> expression)
	:expression(expression.release())
{
}

bool ReturnStatement::execute(Context& context) const
{
	context.setReturnValue(expression->evaluate(context));
	return true;
}

}