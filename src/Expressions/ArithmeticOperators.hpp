#ifndef DS_ARITHMETICOPERATIONS_HPP
#define DS_ARITHMETICOPERATIONS_HPP

#include "Expressions/Expression.hpp"

namespace ds
{

class Addition : public Expression
{
public:
	Addition(const Expression *lhs, const Expression *rhs);
	~Addition();

	float evaluate(const Context& context) const;
private:
	const Expression * const lhs;
	const Expression * const rhs;
};




class Subtraction : public Expression
{
public:
	Subtraction(const Expression *lhs, const Expression *rhs);
	~Subtraction();

	float evaluate(const Context& context) const;
private:
	const Expression * const lhs;
	const Expression * const rhs;
};




class Multiplication : public Expression
{
public:
	Multiplication(const Expression *lhs, const Expression *rhs);
	~Multiplication();

	float evaluate(const Context& context) const;
private:
	const Expression * const lhs;
	const Expression * const rhs;
};




class Division : public Expression
{
public:
	Division(const Expression *lhs, const Expression *rhs);
	~Division();

	float evaluate(const Context& context) const;
private:
	const Expression * const lhs;
	const Expression * const rhs;
};




class UnaryMinus : public Expression
{
public:
	UnaryMinus(const Expression *meat);
	~UnaryMinus();

	float evaluate(const Context& context) const;
private:
	const Expression * const meat;
};

}

#endif
