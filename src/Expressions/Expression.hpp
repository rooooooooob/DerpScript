#ifndef DS_EXPRESSION_HPP
#define DS_EXPRESSION_HPP

#include "Context.hpp"

namespace ds
{

class Context;

class Expression
{
public:
	virtual ~Expression() {}

	virtual float evaluate(const Context& context) const = 0;
protected:
	Expression() {}
};

}

#endif
