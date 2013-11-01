#include "Statements/IfStatement.hpp"

namespace ds
{

IfStatement::IfStatement(const Expression *condition, const Statement *onTrue, const Statement *onFalse)
	:condition(condition)
	,onTrue(onTrue)
	,onFalse(onFalse)
{
}

IfStatement::~IfStatement()
{
	if (condition)
	{
		delete condition;
	}
	if (onTrue)
	{
		delete onTrue;
	}
	if (onFalse)
	{
		delete onFalse;
	}
}

void IfStatement::execute(Context& context) const
{
	if (condition->evaluate(context))
	{
		onTrue->execute(context);
	}
	else if (onFalse)
	{
		onFalse->execute(context);
	}
}

}
