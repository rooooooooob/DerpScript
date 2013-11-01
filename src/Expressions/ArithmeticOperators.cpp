#include "Expressions/ArithmeticOperators.hpp"

namespace ds
{

/*		Addition			*/
Addition::Addition(const Expression *lhs, const Expression *rhs)
	:lhs(lhs)
	,rhs(rhs)
{
}

Addition::~Addition()
{
	delete lhs;
	delete rhs;
}

float Addition::evaluate(const Context& context) const
{
	return lhs->evaluate(context) + rhs->evaluate(context);
}




/*		Subtraction			*/
Subtraction::Subtraction(const Expression *lhs, const Expression *rhs)
	:lhs(lhs)
	,rhs(rhs)
{
}

Subtraction::~Subtraction()
{
	delete lhs;
	delete rhs;
}

float Subtraction::evaluate(const Context& context) const
{
	return lhs->evaluate(context) - rhs->evaluate(context);
}





/*		Multiplication		*/
Multiplication::Multiplication(const Expression *lhs, const Expression *rhs)
	:lhs(lhs)
	,rhs(rhs)
{
}

Multiplication::~Multiplication()
{
	delete lhs;
	delete rhs;
}

float Multiplication::evaluate(const Context& context) const
{
	return lhs->evaluate(context) * rhs->evaluate(context);
}





/*		Division		*/
Division::Division(const Expression *lhs, const Expression *rhs)
	:lhs(lhs)
	,rhs(rhs)
{
}

Division::~Division()
{
	delete lhs;
	delete rhs;
}

float Division::evaluate(const Context& context) const
{
	return lhs->evaluate(context) / rhs->evaluate(context);
}





/*		Unary minus		*/
UnaryMinus::UnaryMinus(const Expression *meat)
	:meat(meat)
{
}

UnaryMinus::~UnaryMinus()
{
	delete meat;
}

float UnaryMinus::evaluate(const Context& context) const
{
	return -(meat->evaluate(context));
}

}
