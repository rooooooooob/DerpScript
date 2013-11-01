#ifndef DS_LOGICALOPERATORS_HPP
#define DS_LOGICALOPERATORS_HPP

#include "Expressions/Expression.hpp"

namespace ds
{

class LogicalAnd : public Expression
{
public:
	LogicalAnd(const Expression *lhs, const Expression *rhs);
	~LogicalAnd();

	float evaluate(const Context& context) const;
private:
	const Expression * const lhs;
	const Expression * const rhs;
};




class LogicalOr : public Expression
{
public:
	LogicalOr(const Expression *lhs, const Expression *rhs);
	~LogicalOr();

	float evaluate(const Context& context) const;
private:
	const Expression * const lhs;
	const Expression * const rhs;
};




class Inverse : public Expression
{
public:
	Inverse(const Expression *meat);
	~Inverse();

	float evaluate(const Context& context) const;
private:
	const Expression * const meat;
};

}

#endif
