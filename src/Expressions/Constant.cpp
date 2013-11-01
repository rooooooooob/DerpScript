#include "Expressions/Constant.hpp"

namespace ds
{

Constant::Constant(float value)
	:Expression()
	,value(value)
{
}

float Constant::evaluate(const Context& context) const
{
	return value;
}

}
