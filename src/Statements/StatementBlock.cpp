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

void StatementBlock::execute(Context& context) const
{
	for (std::vector<Statement*>::const_iterator it = statements.begin(); it != statements.end(); ++it)
	{
		(*it)->execute(context);
	}
}

}
