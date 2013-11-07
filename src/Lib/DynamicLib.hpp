#ifndef DS_DYNAMICLIB_HPP
#define DS_DYNAMICLIB_HPP

namespace ds
{

class Context;

class ParameterList;

extern float eval(Context& context, const ParameterList& params);

extern void exec(Context& context, const ParameterList& params);

extern void load(Context& context, const ParameterList& params);

}

#endif