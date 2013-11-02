#include "FileParser.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <functional>

#include "Statements/Statement.hpp"
#include "SyntaxErrorException.hpp"
#include "Context.hpp"
#include "DSProcedure.hpp"
#include "DSFunction.hpp"
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

	eatWhitespace(filename);

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
	enum class FunctionType
	{
		Procedure,
		Numerical
	};
	FunctionType functionType(FunctionType::Procedure); 

	int line = 0;
	try
	{
		for (line = 0; line < strings.size(); ++line)
		{
			const char *c = strings[line].c_str();

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
				if ((braceCount == 1 && scope.empty()) || (braceCount == 2 && !scope.empty()))
				{
					startOfFunction = line;
				}
			}
			else if (*c == '}')
			{
				--braceCount;
				if (braceCount < 0)
				{
					throw SyntaxErrorException("Found } without matching { before it in file");
				}
				if ((braceCount == 0 && scope.empty()) || (braceCount == 1 && !scope.empty()))
				{
					std::string params;
					for (const ParameterList::Type type : parameterTypes)
					{
						switch (type)
						{
							case ParameterList::Type::Number:
								params += 'N';
								break;
							case ParameterList::Type::String:
								params += 'S';
								break;
						}
					}
					if (scope.empty())
					{
						scope = "local";
					}
					if (functionType == FunctionType::Procedure)
					{
						DSProcedure dsproc(context, parameterNames, parameterTypes, std::unique_ptr<const Statement>(parseStrings(strings, startOfFunction + 1, line)));
						context.registerProcedure(scope, name, params, dsproc);
					}
					else
					{
						DSFunction dsfunc(context, parameterNames, parameterTypes, std::unique_ptr<const Statement>(parseStrings(strings, startOfFunction + 1, line)));
						context.registerFunction(scope, name, params, dsfunc);
					}
				}
				if (braceCount == 0)
				{
					scope.clear();
				}
			}
			else if (strncmp(c, "scope", sizeof("scope") - 1) == 0)
			{
				c += sizeof("scope") - 1;
				eatWhitespace(c);
				while (isalnum(*c))
				{
					scope += *c;
					++c;
				}
			}
			else if (strncmp(c, "func", sizeof("func") - 1) == 0)
			{
				c += sizeof("func") - 1;
				eatWhitespace(c);
				parseProc(name, parameterNames, parameterTypes, c);
				functionType = FunctionType::Numerical;
			}
			else if (strncmp(c, "proc", sizeof("proc") - 1) == 0)
			{
				c += sizeof("proc") - 1;
				eatWhitespace(c);
				parseProc(name, parameterNames, parameterTypes, c);
				functionType = FunctionType::Procedure;
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
			++s;
		}
		else
		{
			if (strncmp(s, "Number", sizeof("Number") - 1) == 0)
			{
				s += sizeof("Number") - 1;
				parameterTypes.push_back(ParameterList::Type::Number);
			}
			else if (strncmp(s, "String", sizeof("String") - 1) == 0)
			{
				s += sizeof("String") - 1;
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
			isParsingName = false;
			if (*s == ')')
			{
				break;
			}
			++s;
			eatWhitespace(s);
		}
	}
}


}