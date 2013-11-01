#ifndef DS_DSPROCEDURE_HPP
#define DS_DSPROCEDURE_HPP

#include <memory>
#include <string>
#include <vector>
#include "Statements/Statement.hpp"
#include "ParameterList.hpp"
#include "Context.hpp"

namespace ds
{

class DSProcedure : public Statement
{
public:
	DSProcedure(
		const std::vector<std::string>& paremterNames,
		std::unique_ptr<const ParameterList> parameters,
		std::unique_ptr<const Statement> body
	);

	void operator()(Context& context) const;

private:
	std::vector<std::string> parameterNames;
	std::unique_ptr<const ParameterList> parameters;
	std::unique_ptr<const Statement> body;
};

}

#endif
