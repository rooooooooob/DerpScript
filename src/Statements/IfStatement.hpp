#ifndef DS_IFSTATEMENT_HPP
#define DS_IFSTATEMENT_HPP

#include "Statements/Statement.hpp"
#include "Expressions/Expression.hpp"

namespace ds
{

class IfStatement : public Statement
{
public:
	IfStatement(const Expression *condition, const Statement *onTrue, const Statement *onFalse = nullptr);
	~IfStatement();

	void execute(Context& context) const;

private:
	const Expression * const condition;
	const Statement * const onTrue;
	const Statement * const onFalse;
};

}

#endif
