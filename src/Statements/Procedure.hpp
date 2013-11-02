#ifndef DS_PROCEDURE_HPP
#define DS_PROCEDURE_HPP

#include <memory>
#include "Statements/Statement.hpp"
#include "ParameterList.hpp"
#include "Context.hpp"

namespace ds
{

class Procedure : public Statement
{
public:
	Procedure(const std::string& scope, const std::string& name, std::unique_ptr<const ParameterList> parameters);

	std::string getSignature() const;

	bool execute(Context& context) const override;
private:
	const std::string scope;
	const std::string name;
	std::unique_ptr<const ParameterList> parameters;
};

}

#endif
