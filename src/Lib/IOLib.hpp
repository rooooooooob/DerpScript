#ifndef DS_IOLIB_HPP
#define DS_IOLIB_HPP

namespace ds
{

class Context;

class ParameterList;

extern void print(Context& context, const ParameterList& params);

extern void printNum(Context& context, const ParameterList& params);

}

#endif