#ifndef DS_RELATIONALOPERATORS_HPP
#define DS_RELATIONALOPERATORS_HPP

#include "Expressions/Expression.hpp"

namespace ds
{

class Equals : public Expression
{
public:
	Equals(const Expression *lhs, const Expression *rhs);
	~Equals();

	float evaluate(const Context& context) const;
private:
	const Expression * const lhs;
	const Expression * const rhs;
};




class LessThan : public Expression
{
public:
	LessThan(const Expression *lhs, const Expression *rhs);
	~LessThan();

	float evaluate(const Context& context) const;
private:
	const Expression * const lhs;
	const Expression * const rhs;
};




class LessThanOrEqual : public Expression
{
public:
	LessThanOrEqual(const Expression *lhs, const Expression *rhs);
	~LessThanOrEqual();

	float evaluate(const Context& context) const;
private:
	const Expression * const lhs;
	const Expression * const rhs;
};




class GreaterThan : public Expression
{
public:
	GreaterThan(const Expression *lhs, const Expression *rhs);
	~GreaterThan();

	float evaluate(const Context& context) const;
private:
	const Expression * const lhs;
	const Expression * const rhs;
};




class GreaterThanOrEqual : public Expression
{
public:
	GreaterThanOrEqual(const Expression *lhs, const Expression *rhs);
	~GreaterThanOrEqual();

	float evaluate(const Context& context) const;
private:
	const Expression * const lhs;
	const Expression * const rhs;
};

}

#endif
