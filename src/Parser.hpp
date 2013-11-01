#ifndef DS_PARSER_HPP
#define DS_PARSER_HPP

#include <vector>
#include "Statements/Statement.hpp"

class Context;

namespace ds
{

extern Statement* parseFile(const char *filename);

extern Statement* parseStrings(const std::vector<std::string>& strings, std::size_t first = 0, std::size_t last = 0);

extern void executeFile(const char *filename, Context& context);

extern void executeStrings(const std::vector<std::string>& strings, Context& context);

}

#endif
