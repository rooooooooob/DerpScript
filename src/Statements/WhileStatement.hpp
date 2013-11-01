#ifndef DS_WHILESTATEMENT_HPP
#define DS_WHILESTATEMENT_HPP

#include "Statements/Statement.hpp"
#include "Expressions/Expression.hpp"

namespace ds
{

class WhileStatement : public Statement
{
public:
	WhileStatement(const Expression *condition, const Statement *body);
	~WhileStatement();

	void execute(Context& context) const;

private:
	const Expression * const condition;
	const Statement * const body;
};

}

#endif

