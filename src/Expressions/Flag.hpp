#ifndef DS_FLAG_HPP
#define DS_FLAG_HPP

#include <string>
#include "Expressions/Expression.hpp"

namespace ds
{

class Flag : public Expression
{
public:
	Flag(const std::string& scope, const std::string& name);

	~Flag() {};

	float evaluate(const Context& context) const;
private:
	const std::string scope;
	const std::string name;
};

}

#endif

