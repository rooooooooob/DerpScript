#ifndef DS_FUNCTIONPARAMETERPARSER_HPP
#define DS_FUNCITONPARAMETERPARSER_HPP

#include <memory>
#include "ParameterList.hpp"

namespace ds
{

extern std::unique_ptr<const ParameterList> parseFunctionParameters(const char *c);

}

#endif
