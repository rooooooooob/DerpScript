#ifndef DS_STATEMENTBLOCK_HPP
#define DS_STATEMENTBLOCK_HPP

#include <vector>
#include "Statements/Statement.hpp"

namespace ds
{

class StatementBlock : public Statement
{
public:
	StatementBlock() {} //  Empty block
	StatementBlock(const std::vector<Statement*>& statements);
	~StatementBlock();

	bool execute(Context& context) const override;
private:
	const std::vector<Statement*> statements;
};

}

#endif
