#include "Statements/SetStringFlag.hpp"

namespace ds
{

SetStringFlag::SetStringFlag(const std::string& scope, const std::string name, std::unique_ptr<const StringConcatenation> expression)
	:scope(scope)
	,name(name)
	,expression(std::move(expression))
{
}

bool SetStringFlag::execute(Context& context) const
{
	context.setStringFlag(scope, name, expression->concatenate(context));
	return false;
}

}
