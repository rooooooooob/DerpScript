#include "Statements/SetString.hpp"

namespace ds
{

SetString::SetString(const std::string& scope, const std::string name, std::unique_ptr<const StringConcatenation> expression)
	:scope(scope)
	,name(name)
	,expression(std::move(expression))
{
}

bool SetString::execute(Context& context) const
{
	context.setString(scope, name, expression->concatenate(context));
	return false;
}

}
