#ifndef DS_DSPROCEDURE_HPP
#define DS_DSPROCEDURE_HPP

#include <memory>
#include <string>
#include <vector>
#include "Statements/Statement.hpp"
#include "ParameterList.hpp"

namespace ds
{

class Context;

class DSProcedure
{
public:
	DSProcedure(Context& context,
		const std::vector<std::string>& parameterNames,
		const std::vector<ParameterList::Type>& parameterTypes,
		std::unique_ptr<const Statement> body
	);

	DSProcedure(DSProcedure& other);

	DSProcedure& operator=(DSProcedure& other);

	void operator()(const ParameterList& parameters) const;

private:

	Context *context;
	std::vector<std::string> parameterNames;
	std::vector<ParameterList::Type> parameterTypes;
	std::unique_ptr<const Statement> body;
};

}

#endif
