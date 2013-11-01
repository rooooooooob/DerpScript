#ifndef DS_STATEMENT_HPP
#define DS_STATEMENT_HPP

#include "Context.hpp"

namespace ds
{

class Statement
{
public:
	virtual ~Statement() {};

	virtual void execute(Context& context) const = 0;

protected:
	Statement() {}
};

}

#endif
