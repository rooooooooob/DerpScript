#ifndef DS_SETSTRINGFLAG_HPP
#define DS_SETSTRINGFLAG_HPP

#include <string>
#include <memory>
#include "Context.hpp"
#include "Statements/Statement.hpp"
#include "StringParser.hpp"
#include "StringConcatenation.hpp"

namespace ds
{

class SetStringFlag : public Statement
{
public:
	SetStringFlag(const std::string& scope, const std::string name, std::unique_ptr<const StringConcatenation> expression);

	bool execute(Context& context) const override;

private:
	const std::string scope;
	const std::string name;
	std::unique_ptr<const StringConcatenation> expression;
};

}

#endif
