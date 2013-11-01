#ifndef DS_VARIABLE_HPP
#define DS_VARIABLE_HPP

#include <string>
#include "Expressions/Expression.hpp"

namespace ds
{

class Variable : public Expression
{
public:
	Variable(const std::string& scope, const std::string& name);

	~Variable() {};

	float evaluate(const Context& context) const;
private:
	const std::string scope;
	const std::string name;
};

}

#endif
