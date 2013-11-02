#ifndef DS_SETVARIABLE_HPP
#define DS_SETVARIABLE_HPP

#include "Statements/Statement.hpp"
#include "Expressions/Expression.hpp"

namespace ds
{

class SetVariable : public Statement
{
public:
	SetVariable(const std::string& scope, const std::string& name, const Expression *expression);

	bool execute(Context& context) const override;

private:
	const std::string scope;
	const std::string name;
	const Expression * const expression;
};

}

#endif
