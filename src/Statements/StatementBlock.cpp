#include "Statements/StatementBlock.hpp"

namespace ds
{

StatementBlock::StatementBlock(const std::vector<Statement*>& statements)
	:statements(statements)
{
}

StatementBlock::~StatementBlock()
{
	for (std::vector<Statement*>::const_iterator it = statements.begin(); it != statements.end(); ++it)
	{
		if (*it)
		{
			delete *it;
		}
	}
}

bool StatementBlock::execute(Context& context) const
{
	for (const Statement *statement : statements)
	{
		if (statement->execute(context))
		{
			return true;
		}
	}
	return false;
}

}
