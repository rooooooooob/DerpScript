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

static void parseProc(std::string& name,
	std::vector<std::string>& parameterNames,
	std::vector<ParameterList::Type>&,
	const char *s
);

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
	std::vector<ParameterList::Type> parameterTypes;
	int startOfFunction = 0;

	int line = 0;
	try
	{
		for (line = 0; line < strings.size(); ++line)
		{
			const char *c = strings[line].c_str();
			eatWhitespace(c);

			if (*c == '\n' || *c == '\r' || *c == '\0')
			{
				continue;
			}

			if (braceCount == 0 && *c != '{' && !scope.empty())
			{
				throw SyntaxErrorException("Expected a {");
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
						for (int j = startOfFunction; j < line; ++j)
						{
							std::cout << "--   " << strings[j] << "   --\n";
						}
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
			else if (strncmp(c, "proc", sizeof("proc")) == 0)
			{
				c += sizeof("proc");
				eatWhitespace(c);
				parseProc(name, parameterNames, parameterTypes, c);
				std::cout << "[][][][]" << name << "(";
				for (int p = 0; p < parameterNames.size(); ++p)
				{
					if (parameterTypes[p] == ParameterList::Type::Number)
						std::cout << "Number";
					else
						std::cout << "String";
					std::cout << parameterNames[p] << ",";
				}
				std::cout << ")\n";
			}
		}
	}
	catch (SyntaxErrorException& see)
	{
		std::stringstream ss;
		ss << see.what()
		   << " on line "
		   << (line + 1)
		   << " of '"
		   << filename
		   << "' not found.";
		throw SyntaxErrorException(ss.str());
	}
}

void parseProc(std::string& name,
	std::vector<std::string>& parameterNames,
	std::vector<ParameterList::Type>& parameterTypes,
	const char *s
)
{
	name.clear();
	parameterNames.clear();
	parameterTypes.clear();

	eatWhitespace(s);
	while (isalnum(*s))
	{
		name += *s;
		++s;
	}
	eatWhitespace(s);
	
	if (*s == '(')
	{
		++s;
		eatWhitespace(s);
	}
	else
	{
		std::stringstream ss;
		ss << "Encountered unexpected character '"
		   << *s
		   << "' instead of a '(' in procedure declaration";
		throw SyntaxErrorException(ss.str());
	}

	bool isParsingName = false;
	for ( ; ; )
	{
		if (*s == '\0')
		{
			throw SyntaxErrorException("Hit end of line while still parsing procedure arguments");
		}

		if (isParsingName)
		{
			parameterNames.back() += *s;
		}
		else
		{
			if (strncmp(s, "Number", sizeof("Number")) == 0)
			{
				s += sizeof("Number");
				parameterTypes.push_back(ParameterList::Type::Number);
			}
			else if (strncmp(s, "String", sizeof("String")) == 0)
			{
				s += sizeof("String");
				parameterTypes.push_back(ParameterList::Type::String);
			}
			else
			{
				throw SyntaxErrorException("Unknown parameter type found");
			}
			isParsingName = true;
			eatWhitespace(s);
			parameterNames.push_back("");
		}

		if (*s == ',' || *s == ')')
		{
			++s;
			eatWhitespace(s);
			isParsingName = false;
			if (*s == ')')
			{
				break;
			}
		}
	}
}


}