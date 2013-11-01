#ifndef DS_EXPRESSIONPARSER_HPP
#define DS_EXPRESSIONPARSER_HPP

#include "Expressions/Expression.hpp"
#include "Context.hpp"

namespace ds
{

extern Expression* parseExpression(const char *exp);

}

#endif
