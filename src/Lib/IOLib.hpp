#ifndef DS_IOLIB_HPP
#define DS_IOLIB_HPP

#include <string>

namespace ds
{

class Context;

class ParameterList;

extern void print(Context& context, const ParameterList& params);

extern void printNum(Context& context, const ParameterList& params);

extern void println(Context& context, const ParameterList& params);

extern void printlnNum(Context& context, const ParameterList& params);

extern std::string read(Context& context, const ParameterList& params);

extern std::string readln(Context& context, const ParameterList& params);

extern std::string fileread(Context& context, const ParameterList& params);

extern std::string filereadln(Context& context, const ParameterList& params);

extern void filewrite(Context& context, const ParameterList& params);

extern void open(Context& context, const ParameterList& params);

extern void close(Context& context, const ParameterList& params);

}

#endif