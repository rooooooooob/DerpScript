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

bool IfStatement::execute(Context& context) const
{
	if (condition->evaluate(context))
	{
		if (onTrue->execute(context))
		{
			return true;
		}
	}
	else if (onFalse)
	{
		if (onFalse->execute(context))
		{
			return true;
		}
	}
	return false;
}

}
