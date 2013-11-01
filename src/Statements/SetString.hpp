#ifndef DS_SETSTRING_HPP
#define DS_SETSTRING_HPP

#include <string>
#include <memory>
#include "Context.hpp"
#include "Statements/Statement.hpp"
#include "StringParser.hpp"
#include "StringConcatenation.hpp"

namespace ds
{

class SetString : public Statement
{
public:
	SetString(const std::string& scope, const std::string name, std::unique_ptr<const StringConcatenation> expression);

	void execute(Context& context) const;

private:
	const std::string scope;
	const std::string name;
	std::unique_ptr<const StringConcatenation> expression;
};

}

#endif
