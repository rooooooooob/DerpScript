#ifndef DS_DSPROCEDURE_HPP
#define DS_DSPROCEDURE_HPP

#include <memory>
#include "Statements/Statement.hpp"
#include "ParameterList.hpp"
#include "Context.hpp"

namespace ds
{

class DSProcedure : public Statement
{
public:
	DSProcedure(std::unique_ptr<const Statement> body);

	void operator()(Context& context) const;

private:
	std::unique_ptr<const Statement> body;
};

}

#endif
