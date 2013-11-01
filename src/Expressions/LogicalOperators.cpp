#include "Expressions/LogicalOperators.hpp"

namespace ds
{

/*			Logical AND			*/
LogicalAnd::LogicalAnd(const Expression *lhs, const Expression *rhs)
	:Expression()
	,lhs(lhs)
	,rhs(rhs)
{
}

LogicalAnd::~LogicalAnd()
{
	delete lhs;
	delete rhs;
}

float LogicalAnd::evaluate(const Context& context) const
{
	return (lhs->evaluate(context) && rhs->evaluate(context));
}





/*			Logical OR			*/
LogicalOr::LogicalOr(const Expression *lhs, const Expression *rhs)
	:Expression()
	,lhs(lhs)
	,rhs(rhs)
{
}

LogicalOr::~LogicalOr()
{
	delete lhs;
	delete rhs;
}

float LogicalOr::evaluate(const Context& context) const
{
	return (lhs->evaluate(context) || rhs->evaluate(context));
}





/*			Logical NOT		*/
Inverse::Inverse(const Expression *meat)
	:meat(meat)
{
}

Inverse::~Inverse()
{
	delete meat;
}

float Inverse::evaluate(const Context& context) const
{
	return (meat->evaluate(context) == 0);
}

}
