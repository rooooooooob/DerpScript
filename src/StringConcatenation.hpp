#ifndef DS_STRINGCONCATENATION_HPP
#define DS_STRINGCONCATENATION_HPP

#include <vector>
#include <string>
//#include "Expressions/Function.hpp"
//#include "Context.hpp"

namespace ds
{

class Context;

class Function;

class StringConcatenation
{
public:
	StringConcatenation();
	~StringConcatenation();

	void addStringLiteral(const std::string& stringLiteral);

	void addStringFunction(const Function *function);

	std::string concatenate(const Context& context) const;
private:
	enum Type
	{
		StringLiteral,
		StringFunction
	};
	std::vector<const Function*> functions;
	std::vector<std::string> literals;
	std::vector<Type> types;
};

}

#endif
