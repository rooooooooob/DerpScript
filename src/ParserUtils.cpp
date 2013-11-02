#include "ParserUtils.hpp"

namespace ds
{

void eatWhitespace(const char*& c)
{
	if (*c != '\0')
	{
		while ((*c == ' ' || *c == '\t') && *(c + 1) != '\0')
		{
			++c;
		}
	}
}

}