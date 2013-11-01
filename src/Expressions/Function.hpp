#ifndef DS_FUNCTION_HPP
#define DS_FUNCTION_HPP

#include <memory>
#include "Expressions/Expression.hpp"
//#include "Context.hpp"

namespace ds
{

class ParameterList;

class Function : public Expression
{
public:
	Function(const std::string& scope, const std::string& name, std::unique_ptr<const ParameterList> parameters);


	float evaluate(const Context& context) const;

	std::string evaluateAsString(const Context& context) const;


private:
	Function(Function& other);
	Function& operator=(Function& other);


	const std::string scope;
	const std::string name;
	std::unique_ptr<const ParameterList> parameters;
};

}

#endif // DS_FUNCTION_HPP
