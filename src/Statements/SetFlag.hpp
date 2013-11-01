#ifndef DS_SETFLAG_HPP
#define DS_SETFLAG_HPP

#include <memory>
#include "Statements/Statement.hpp"
#include "Expressions/Expression.hpp"

namespace ds
{

class SetFlag : public Statement
{
public:
	SetFlag(const std::string& scope, const std::string& name, const Expression *expression);

	void execute(Context& context) const;

private:
	const std::string scope;
	const std::string name;
	//const Expression * const expression;
	std::unique_ptr<const Expression> expression;
};

}

#endif

