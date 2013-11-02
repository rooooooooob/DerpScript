#include "FileParser.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>

#include "SyntaxErrorException.hpp"
#include "Context.hpp"
#include "Statements/DSProcedure.hpp"
#include "ParameterList.hpp"
#include "Parser.hpp"
#include "ParserUtils.hpp"

namespace ds
{

void loadFile(Context& context, const char *filename)
{
	std::vector<std::string> strings;

	std::fstream file(filename, std::fstream::in);

	if (file.is_open())
	{
		std::cout << "Opened '" << filename << "'\n";
		while (file.good())
		{
			strings.push_back(std::string());
			getline(file, strings.back());
			std::cout << strings.size();
			if (strings.size() < 10)
				std::cout << " ";
			std::cout << " | " << strings.back() << std::endl;
		}
		file.close();
	}
	else
	{
		std::cerr << "Couldn't open '" << filename << "'\n";
	}

	int braceCount = 0;
	std::string scope;
	std::string name;
	std::vector<std::string> parameterNames;
	std::vector<std::string> parameterTypes;
	int startOfFunction = 0;

	for (int line = 0; line < strings.size(); ++line)
	{
		const char *c = strings[line].c_str();
		eatWhitespace(c);

		if (*c == '\n' || *c == '\r' || *c == '\0')
		{
			continue;
		}

		if (braceCount == 0 && *c != '{' && !scope.empty())
		{
			std::stringstream ss;
			ss << "Expected { on line " << (line + 1) << " of '" << filename << "' not found.";
			throw SyntaxErrorException(ss.str());
		}

		if (*c == '{')
		{
			++braceCount;
		}
		else if (*c == '}')
		{
			--braceCount;
			if (scope.empty())
			{
				if (braceCount == 0)
				{
					//statement = parseSrings(strings, startOfFunction, line);
					for (int j = startOfFunction; j < line; ++j)
					{
						std::cout << "--   " << strings[j] << "   --\n";
					}
				}
			}
			else
			{
				if (braceCount == 0)
				{
					scope.clear();
				}
				else if (braceCount == 1)
				{

				}
			}
		}
		else if (strncmp(c, "scope", sizeof("scope")) == 0)
		{
			c += sizeof("scope");
			eatWhitespace(c);
			scope = c;
		}
		else if (strncmp(c, "func", sizeof("func")) == 0)
		{
			c += sizeof("func");
			eatWhitespace(c);
			
		}
	}
}



}