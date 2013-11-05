#include "Lib/StringLib.hpp"

#include <cmath>
#include "Context.hpp"
#include "ParameterList.hpp"

namespace ds
{

float dssqrt(Context& context, const ParameterList& params)
{
	return sqrtf(params.getNumericalParameter(context, 0));
}

float dsceil(Context& context, const ParameterList& params)
{
	return ceilf(params.getNumericalParameter(context, 0));
}

float dsfloor(Context& context, const ParameterList& params)
{
	return floorf(params.getNumericalParameter(context, 0));
}

float dsround(Context& context, const ParameterList& params)
{
	return ceilf(params.getNumericalParameter(context, 0));
}

extern float dspow(Context& context, const ParameterList& params)
{
	return powf(params.getNumericalParameter(context, 0), params.getNumericalParameter(context, 1));
}

extern float dssin(Context& context, const ParameterList& params)
{
	return sinf(params.getNumericalParameter(context, 0));
}

extern float dscos(Context& context, const ParameterList& params)
{
	return cosf(params.getNumericalParameter(context, 0));
}

}