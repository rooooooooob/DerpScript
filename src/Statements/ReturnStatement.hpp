#ifndef DS_RETURNSTATEMENT_HPP
#define DS_RETURNSTATEMENT_HPP

#include <memory>
#include "Statements/Statement.hpp"
#include "Expressions/Expression.hpp"

namespace ds
{

class ReturnStatement : public Statement
{
public:
	ReturnStatement(std::unique_ptr<const Expression> expression);

	bool execute(Context& context) const override;

private:
	std::unique_ptr<const Expression> expression;
};

}

#endif