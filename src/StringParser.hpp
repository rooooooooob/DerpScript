#ifndef DS_STRINGPARSER_HPP
#define DS_STRINGPARSER_HPP

#include <memory>
#include "StringConcatenation.hpp"

namespace ds
{

extern std::unique_ptr<const StringConcatenation> parseStringExpression(const char *exp);

}

#endif
