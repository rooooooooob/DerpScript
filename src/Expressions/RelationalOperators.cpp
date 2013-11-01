#include "Expressions/RelationalOperators.hpp"

namespace ds
{

/*			Equal to				*/
Equals::Equals(const Expression *lhs, const Expression *rhs)
	:Expression()
	,lhs(lhs)
	,rhs(rhs)
{
}

Equals::~Equals()
{
	delete lhs;
	delete rhs;
}

float Equals::evaluate(const Context& context) const
{
	return (lhs->evaluate(context) == rhs->evaluate(context));
}





/*			Less than				*/
LessThan::LessThan(const Expression *lhs, const Expression *rhs)
	:Expression()
	,lhs(lhs)
	,rhs(rhs)
{
}

LessThan::~LessThan()
{
	delete lhs;
	delete rhs;
}

float LessThan::evaluate(const Context& context) const
{
	return (lhs->evaluate(context) < rhs->evaluate(context));
}





/*			Less than or equal		*/
LessThanOrEqual::LessThanOrEqual(const Expression *lhs, const Expression *rhs)
	:Expression()
	,lhs(lhs)
	,rhs(rhs)
{
}

LessThanOrEqual::~LessThanOrEqual()
{
	delete lhs;
	delete rhs;
}

float LessThanOrEqual::evaluate(const Context& context) const
{
	return (lhs->evaluate(context) <= rhs->evaluate(context));
}





/*			Greater than			*/
GreaterThan::GreaterThan(const Expression *lhs, const Expression *rhs)
	:Expression()
	,lhs(lhs)
	,rhs(rhs)
{
}

GreaterThan::~GreaterThan()
{
	delete lhs;
	delete rhs;
}

float GreaterThan::evaluate(const Context& context) const
{
	return (lhs->evaluate(context) > rhs->evaluate(context));
}





/*			Greater than or equal	*/
GreaterThanOrEqual::GreaterThanOrEqual(const Expression *lhs, const Expression *rhs)
	:Expression()
	,lhs(lhs)
	,rhs(rhs)
{
}

GreaterThanOrEqual::~GreaterThanOrEqual()
{
	delete lhs;
	delete rhs;
}

float GreaterThanOrEqual::evaluate(const Context& context) const
{
	return (lhs->evaluate(context) >= rhs->evaluate(context));
}

}
