#include "Statements/WhileStatement.hpp"

namespace ds
{

WhileStatement::WhileStatement(const Expression *condition, const Statement *body)
	:condition(condition)
	,body(body)
{
}

WhileStatement::~WhileStatement()
{
	delete condition;
	delete body;
}

bool WhileStatement::execute(Context& context) const
{
	while (condition->evaluate(context))
	{
		if (body->execute(context))
		{
			return true;
		}
	}
	return false;
}

}

