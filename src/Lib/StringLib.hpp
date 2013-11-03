#ifndef DS_STRINGLIB_HPP
#define DS_STRINGLIB_HPP

#include <string>

namespace ds
{

class Context;

class ParameterList;

extern std::string str(Context& context, const ParameterList& params);

extern float strlen(Context& context, const ParameterList& params);

extern float num(Context& context, const ParameterList& params);

}

#endif