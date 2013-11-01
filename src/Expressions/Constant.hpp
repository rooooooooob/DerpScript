#ifndef DS_CONSTANT_HPP
#define DS_CONSTANT_HPP

#include "Expressions/Expression.hpp"

namespace ds
{

class Constant : public Expression
{
public:
	Constant(float value);
	~Constant() {};

	float evaluate(const Context& context) const;
private:
	const float value;
};

}

#endif
