#include "Expressions/Variable.hpp"

namespace ds
{

Variable::Variable(const std::string& scope, const std::string& name)
	:scope(scope)
	,name(name)
{
}

float Variable::evaluate(const Context& context) const
{
	return context.getVar(scope, name);
}

}
