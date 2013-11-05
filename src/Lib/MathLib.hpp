#ifndef DS_MATHLIB_HPP
#define DS_MATHLIB_HPP

#include <string>

namespace ds
{

class Context;

class ParameterList;


extern float dssqrt(Context& context, const ParameterList& params);

extern float dsceil(Context& context, const ParameterList& params);

extern float dsfloor(Context& context, const ParameterList& params);

extern float dsround(Context& context, const ParameterList& params);

extern float dspow(Context& context, const ParameterList& params);

extern float dssin(Context& context, const ParameterList& params);

extern float dscos(Context& context, const ParameterList& params);

}

#endif