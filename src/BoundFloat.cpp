#include "BoundFloat.hpp"

namespace ds
{

BoundFloat::BoundFloat(float& reference)
	:variable(reference)
{
}

void BoundFloat::set(float value)
{
	variable = value;
}

float BoundFloat::get() const
{
	return variable;
}

}
