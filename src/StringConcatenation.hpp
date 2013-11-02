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

	void addStringVariable(const std::string& scope, const std::string& name);

	void addStringFlag(const std::string&, const std::string& name);

	std::string concatenate(const Context& context) const;
private:
	enum class Type
	{
		StringLiteral,
		StringFunction,
		StringVariable,
		StringFlag
	};
	std::vector<const Function*> functions;
	std::vector<std::string> literals;//used for names too
	std::vector<std::string> scopes;
	std::vector<Type> types;
};

}

#endif
