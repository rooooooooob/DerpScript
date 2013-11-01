#include "Statements/DSProcedure.hpp"

namespace ds
{

DSProcedure::DSProcedure(std::unique_ptr<const Statement> body)
	:body(body.release())
{
}

void DSProcedure::operator()(Context& context) const
{
	body->execute(context);
}

}