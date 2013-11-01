#include "Statements/Procedure.hpp"

namespace ds
{

Procedure::Procedure(const std::string& scope, const std::string& name, std::unique_ptr<const ParameterList> parameters)
	:scope(scope)
	,name(name)
	,parameters(parameters.release())
{
}

std::string Procedure::getSignature() const
{
	return scope + ":" + name + parameters->getFormattedSignature();
}

void Procedure::execute(Context& context) const
{
	context.executeProcedure(scope, name, *parameters);
}

}
