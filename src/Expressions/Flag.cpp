#include "Expressions/Flag.hpp"

namespace ds
{

Flag::Flag(const std::string& scope, const std::string& name)
	:scope(scope)
	,name(name)
{
}

float Flag::evaluate(const Context& context) const
{
	return context.getFlag(scope, name);
}

}

