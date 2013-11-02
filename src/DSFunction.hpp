#ifndef DS_DSFUNCTION_HPP
#define DS_DSFUNCTION_HPP

#include <memory>
#include <string>
#include <vector>
#include "Statements/Statement.hpp"
#include "ParameterList.hpp"

namespace ds
{

class Context;

class DSFunction
{
public:
	DSFunction(Context& context,
		const std::vector<std::string>& parameterNames,
		const std::vector<ParameterList::Type>& parameterTypes,
		std::unique_ptr<const Statement> body
	);

	DSFunction(DSFunction& other);

	DSFunction& operator=(DSFunction& other);

	float operator()(const ParameterList& parameters) const;

private:

	Context *context;
	std::vector<std::string> parameterNames;
	std::vector<ParameterList::Type> parameterTypes;
	std::unique_ptr<const Statement> body;
};

}

#endif
