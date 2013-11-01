#include "StringParser.hpp"

#include "SyntaxErrorException.hpp"
#include "Expressions/Function.hpp"
#include "FunctionParameterParser.hpp"

#include <iostream>

namespace ds
{

static void eatWhitespace(const char*& c);

/*		Extern functiosn		*/
std::unique_ptr<const StringConcatenation> parseStringExpression(const char *exp)
{
	std::unique_ptr<StringConcatenation> returnVal(new StringConcatenation());

	bool parsingLiteral = false;
	bool parsingFunction = false;
	bool escapeChar = false;
	int squareBracketDepth = 0;

	const char *c = exp;
	const char *start;
	const char *startOfStringFunction = nullptr;
	std::cout << "we got [" << c << "]" << std::endl;

	std::string buffer;	//	used to hold string literals

	while (*c != '\0')
	{
		if (parsingLiteral)
		{
			if (*c == '\0')
			{
				//	error: no end to string literal
				throw SyntaxErrorException("No ending \" to string literal");
			}
			else if (escapeChar)
			{
				if (*c == '"')
				{
					buffer += '"';
				}
				else if (*c == '\\')
				{
					buffer += '\\';
				}
				else if (*c == '\n')
				{
					buffer += '\n';
				}
				else
				{
					//	error: invalid escape char
					std::string error = "invalid escpe char: '";
					error += *c;
					error += '\'';
					throw SyntaxErrorException(error);
				}
				escapeChar = false;
			}
			else if (*c == '"')
			{
				returnVal->addStringLiteral(buffer);
				parsingLiteral = false;
				buffer.clear();
			}
			else if (*c == '\\')
			{
				escapeChar = true;
			}
			else
			{
				buffer += *c;
			}
		}
		else
		{
			if (!parsingFunction && *c == '"')
			{
				parsingLiteral = true;
			}
			else if (*c == '[')
			{
				if (++squareBracketDepth == 1)
				{
					eatWhitespace(c);
					startOfStringFunction = c + 1;
				}
				parsingFunction = true;
			}
			else if (*c == ']')
			{
				if (--squareBracketDepth == 0)
				{
					parsingFunction = false;
					std::string scope, name;
					const char * const end = c;
					c = startOfStringFunction;
					if ((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z'))
					{
						while ((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z'))
						{
							scope += *c;
							++c;
						}
						if (*c == ':')
						{
							while ((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z'))
							{
								++c;
							}
						}
						if (*c == '(')
						{
							if (name.empty())
							{
								name = scope;
								scope = "local";
							}
							std::string buffer;
							int parenthesisDepth = 1;
							for ( ; ; )
							{
								++c;
								if (*c == ')')
								{
									if (--parenthesisDepth == 0)
									{
										break;
									}
								}
								else if (*c == '(')
								{
									++parenthesisDepth;
								}
								else if (*c == '\0')
								{
									throw SyntaxErrorException("No end to function args");
								}
								buffer += *c;
							}
							returnVal->addStringFunction(new Function(scope, name, parseFunctionParameters(buffer.c_str())));
							c = end;
						}
						else
						{
							throw SyntaxErrorException("StringParser.cpp : unknown symbol in id");
						}
					}
					else
					{
						throw SyntaxErrorException("too lazy to write, but invalid string function name in StringParser.cpp");
					}
				}
				else if (squareBracketDepth < 0)
				{
					//	error:
					throw SyntaxErrorException("too lazy to write this now, but in StringParser.cpp you have too many ]s");
				}
			}
		}
		++c;
	}

	return std::unique_ptr<const StringConcatenation>(returnVal.release());
}

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
