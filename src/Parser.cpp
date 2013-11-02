#include "Parser.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstring>

#include "ExpressionParser.hpp"
#include "StringParser.hpp"
#include "SyntaxErrorException.hpp"
#include "FunctionParameterParser.hpp"
#include "ParserUtils.hpp"

#include "Statements/IfStatement.hpp"
#include "Statements/WhileStatement.hpp"
#include "Statements/StatementBlock.hpp"
#include "Statements/SetVariable.hpp"
#include "Statements/SetFlag.hpp"
#include "Statements/SetString.hpp"
#include "Statements/Procedure.hpp"

//  Because of +=, -=, *=, and /=
#include "Expressions/ArithmeticOperators.hpp"

#include "Expressions/Variable.hpp"
#include "Expressions/Flag.hpp"

namespace ds
{

///////////////////////////////////////////////////////////////////
//		  DECLARATION OF STUFF USED ONLY IN THIS FILE		   //
///////////////////////////////////////////////////////////////////

enum TokenType {If, Else, While, SetVar, StringSet, ProcedureCall, OpenBrace, CloseBrace};

struct SToken
{
	SToken(TokenType type, std::size_t lineNumber, const std::string& line);

	TokenType type;
	Statement *statement;//in the case of constants, vars, and flags
	std::size_t lineNumber;
	std::string line;
	Expression *expression;
};


struct SNode
{
	SNode(SToken type, SNode*& head);

	Statement* createStatement();
	void killLeft();
	void killRight();

	TokenType type;
	Statement *statement;
	SNode *left, *right;
	SNode*& head;
	const std::size_t lineNumber;
	const std::string line;
	const Expression *expression;
};

///////////////////////////////////////////////////
//         static function prototypes            //
///////////////////////////////////////////////////


//  This parser is such a mess...

static void createTokens(std::vector<SToken>& tokens, const std::vector<std::string>& strings, std::size_t start, std::size_t end);

static Statement *parseTokens(const std::vector<SToken>& tokens, std::size_t start, std::size_t end);

static char tokenToChar(TokenType type);

////////////////////////////////////////////////////
//       PUBLIC FUNCTION DEFINITIONS              //	<--------EXTERN-DEFINITIONS-HERE---------<<<
////////////////////////////////////////////////////



Statement* parseFile(const char *filename)
{
	std::vector<std::string> strings;

	eatWhitespace(filename);

	std::fstream file(filename, std::fstream::in);

	if (file.is_open())
	{
		//std::cout << "Opened '" << filename << "'\n";
		while (file.good())
		{
			strings.push_back(std::string());
			getline(file, strings.back());
			//std::cout << strings.size();
			//if (strings.size() < 10)
			//	std::cout << " ";
			//std::cout << " | " << strings.back() << std::endl;
		}
		file.close();
	}
	else
	{
		std::cerr << "Couldn't open '" << filename << "'\n";
	}
	Statement *logic;
	try
	{
		logic = parseStrings(strings);
	}
	catch (const SyntaxErrorException& see)
	{
		throw SyntaxErrorException(see, " in " + std::string(filename));
	}
	return logic;
}

Statement* parseStrings(const std::vector<std::string>& strings, std::size_t first, std::size_t last)
{
	if (last == 0)
	{
		last = strings.size();
	}

	std::vector<SToken> tokens;

	Statement *retVal = nullptr;

	try
	{
		createTokens(tokens, strings, first, last);

		retVal = parseTokens(tokens, 0, tokens.size());
	}
	catch (const SyntaxErrorException& see)
	{
		//  free memory so we don't leak
		for (std::vector<SToken>::iterator it = tokens.begin(); it != tokens.end(); ++it)
		{
			if (it->statement)
			{
				delete it->statement;
			}
			if (it->expression)
			{
				delete it->expression;
			}
		}
		throw see;
	}

	if (!retVal)
	{
		//  I hope this never happens
		retVal = new StatementBlock();  //  empty block
	}

	return retVal;
}

void executeFile(const char *filename, Context& context)
{
	Statement *logic = nullptr;

	logic = parseFile(filename);

	logic->execute(context);

	if (logic)
	{
		delete logic;
	}
}

void executeStrings(const std::vector<std::string>& strings, Context& context)
{
	Statement *logic = parseStrings(strings);

	logic->execute(context);

	delete logic;
}

//////////////////////////////////////////////////
//		  struct method definitions		   //	  <-----NOW-MORE-LOCAL-STUFF-----<<<
//////////////////////////////////////////////////

SToken::SToken(TokenType type, std::size_t lineNumber, const std::string& line)
	:type(type)
	,statement(nullptr)
	,lineNumber(lineNumber)
	,line(line)
	,expression(nullptr)
{
}



SNode::SNode(SToken type, SNode*& head)
	:type(type.type)
	,statement(type.statement)
	,left(nullptr)
	,right(nullptr)
	,head(head)
	,lineNumber(type.lineNumber)
	,line(type.line)
	,expression(type.expression)
{
}

Statement* SNode::createStatement()
{
	if (!statement)
	{
		if (type == If)
		{
			//  I don't think it's physically possible to cram more 'right's into the next couple lines...
			if (right && right->right && right->right->right && right->right->type == Else)
			{
				statement = new IfStatement(expression, right->createStatement(), right->right->right->createStatement());
				expression = nullptr;
				right->right->killRight();
				right->killRight();
				killRight();
			}
			else
			{
				if (!right)
				{
					std::stringstream ss;
					ss << "If statement followed by no other statements: \""
					   << line
					   << "\" on line #"
					   << lineNumber;

					throw SyntaxErrorException(ss.str());
				}
				statement = new IfStatement(expression, right->createStatement());
				expression = nullptr;
				killRight();
			}
		}
		else if (type == While)
		{
			if (right)
			{
				statement = new WhileStatement(expression, right->createStatement());
				expression = nullptr;
				killRight();
			}
			else
			{
				std::stringstream ss;
				ss << "While statement followed by no other statements: \""
					<< line
					<< "\" on line #"
					<< lineNumber;

				throw SyntaxErrorException(ss.str());
			}
		}
	}
	Statement *retVal = statement;
	statement = nullptr;   //  transfer ownership
	return retVal;
}

void SNode::killLeft()
{
	if (!left)
	{
		return;
	}
	if (left == head)
	{
		head = this;
	}
	SNode *temp = left;
	if (left->left)
	{
		left->left->right = this;
	}
	left = left->left;
	//std::cout << "killed left\n";
	if (temp->statement)
	{
		delete temp->statement;
	}
	if (temp->expression)
	{
		delete temp->expression;
	}
	delete temp;
}

void SNode::killRight()
{
	if (!right)
	{
		//std::cout << "couldn't kill right (\n";
		return;
	}
	SNode *temp = right;
	if (right->right)
	{
		right->right->left = this;
	}
	right = right->right;
	//std::cout << "killed right\n";
	if (temp->statement)
	{
		delete temp->statement;
	}
	if (temp->expression)
	{
		delete temp->expression;
	}
	delete temp;
}


/*		  END OF TOKEN METHODS			*/


////////////////////////////////////////////////////
//		  STATIC FUNCTION DEFINITIONS		   //
////////////////////////////////////////////////////


void createTokens(std::vector<SToken>& tokens, const std::vector<std::string>& strings, std::size_t first, std::size_t last)
{
	tokens.clear(); //  memory leaks lol
	bool parsedElse;
	//std::cout << "Parsing statements: ";
	for (std::size_t line = first; line < last; ++line)
	{
		const char *c = strings[line].c_str();
		parsedElse = false;

		//std::cout << "[" << strings[line] << "]";
		//std::cout.flush();

		eatWhitespace(c);
		if (*c == '{')
		{
			tokens.push_back(SToken(OpenBrace, line + 1, strings[line]));
			continue;
		}
		else if (*c == '}')
		{
			tokens.push_back(SToken(CloseBrace, line + 1, strings[line]));
			continue;
		}
		if (strncmp(c, "else", 4) == 0)
		{
			tokens.push_back(SToken(Else, line + 1, strings[line]));
			c += 4;
			parsedElse = true;
		}
		eatWhitespace(c);
		if (strncmp(c, "if", 2) == 0)
		{
			tokens.push_back(SToken(If, line + 1, strings[line]));
			c += 2;
			tokens.back().expression = parseExpression(c);
		}
		else if (strncmp(c, "while", 5) == 0)
		{
			tokens.push_back(SToken(While, line + 1, strings[line]));
			c += 5;
			tokens.back().expression = parseExpression(c);
		}
		else if (strncmp(c, "set", 3) == 0)	//	string setters
		{
			tokens.push_back(SToken(StringSet, line + 1, strings[line]));
			bool onScope = true;
			std::string scope, name;
			c += 3;
			eatWhitespace(c);
			while ((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z') || *c == '.')
			{
				if (*c == '.')
				{
					onScope = false;
				}
				else if (onScope)
				{
					scope += *c;
				}
				else
				{
					name += *c;
				}
				++c;
			}
			eatWhitespace(c);
			tokens.back().statement = new SetString(scope, name, parseStringExpression(c));
		}
		else if ((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z'))  //  variables / flags
		{
			bool onScope = true;
			bool isFlag = false;
			std::string scope, name;
			while ((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z') || *c == '.' || *c == ':')
			{
				if (*c == ':')
				{
					onScope = false;
					isFlag = true;
				}
				else if (*c == '.')
				{
					onScope = false;
				}
				else if (onScope)
				{
					scope += *c;
				}
				else
				{
					name += *c;
				}
				++c;
			}
			if (onScope)	//  local var
			{
				name = scope;
				scope = "local";
				isFlag = true;
			}
			eatWhitespace(c);
			if (*c == '(')	// might be a function
			{
				tokens.push_back(SToken(ProcedureCall, line + 1, strings[line]));
				int depthLevel = 0;
				const char *startOfParam = c + 1;
				bool isInStringLiteral = false;
				while (*c != '\0')
				{
					if (isInStringLiteral)
					{
						if (*c == '"')
						{
							isInStringLiteral = false;
						}
					}
					else
					{
						if (*c == '"')
						{
							isInStringLiteral = true;
						}
						else if (*c == '(')
						{
							++depthLevel;
						}
						else if (*c == ')')
						{
							if (--depthLevel <= 0)
							{
								std::string buffer(startOfParam, c - startOfParam);
								//std::cout << "arglist of " << scope << "." << name << "() is [" << buffer << "]" << std::endl;
								tokens.back().statement = new Procedure(scope, name, parseFunctionParameters(buffer.c_str()));
								break;
							}
						}
					}
					++c;
				}
			}
			else
			{
				tokens.push_back(SToken(SetVar, line + 1, strings[line]));
				while (*c != '=' && *c != '\0')
				{
					++c;
				}
				if (*c == '\0')
				{
					std::stringstream ss;
					ss << "Missing expression in \""
					   << strings[line]
					   << "\" on line #"
					   << (line + 1);
					throw SyntaxErrorException(ss.str());
				}
				++c;
				Expression *exp = parseExpression(c);
				Expression *self;
				if (isFlag)
				{
					self = new Flag(scope, name);
				}
				else
				{
					self = new Variable(scope, name);
				}
				if (*(c - 2) == '+')
				{
					exp = new Addition(self, exp);
				}
				else if (*(c - 2) == '-')
				{
					exp = new Subtraction(self, exp);
				}
				else if (*(c - 2) == '*')
				{
					exp = new Multiplication(self, exp);
				}
				else if (*(c - 2) == '/')
				{
					exp = new Division(self, exp);
				}
				eatWhitespace(c);
				if (isFlag)
				{
					tokens.back().statement = new SetFlag(scope, name, exp);
				}
				else
				{
					tokens.back().statement = new SetVariable(scope, name, exp);
				}
			}
		}
		else if (!parsedElse && strncmp(c, "//", 2) != 0)	//  Uh oh, not a comment either...
		{
			std::stringstream ss;
			ss << "Encountered unknown statement \""
			   << strings[line]
			   << "\" on line #"
			   << (line + 1);
			throw SyntaxErrorException(ss.str());
		}
	}
	//std::cout << "\ndone creating tokens" << std::endl;
}

Statement* parseTokens(const std::vector<SToken>& tokens, std::size_t start, std::size_t end)
{
	//std::cout << "parseTokens() on:";
	/*for (std::size_t i = start; i < end; ++i)
	{
		std::cout << tokenToChar(tokens[i].type) << " ";
	}*/
	//std::cout << std::endl;
	if (tokens.empty())
	{
		//std::cout << "empty tokens?\n";
		return new StatementBlock();
	}
	SNode *head = nullptr, *cur = nullptr, *temp;
	std::vector<Statement*> statements;
	try
	{
		//handle curly-brace fuckery
		for (std::size_t i = start; i < end; ++i)
		{
			//std::cout << tokenToChar(tokens[i].type) << ",";
			if (tokens[i].type == CloseBrace)
			{
				continue;
			}
			temp = new SNode(tokens[i], head);
			if (!head)
			{
				head = temp;
			}
			if (cur)
			{
				cur->right = temp;
				temp->left = cur;
			}
			cur = temp;
			if (tokens[i].type == OpenBrace)
			{
				std::size_t newStart = i + 1;
				int depth = 0;
				while (i < end)
				{
					if (tokens[i].type == OpenBrace)
					{
						++depth;
					}
					else if (tokens[i].type == CloseBrace)
					{
						--depth;
						if (depth == 0)
						{
							break;
						}
					}
					++i;
				}
				//skip ahead until CloseBrace then call this on that
				cur->statement = parseTokens(tokens, newStart, i);
			}
		}
		//std::cout << "{"; std::cout.flush();
		for (SNode *it = head; it != nullptr; it = it->right)
		{
			//std::cout << "eval" << tokenToChar(it->type) << std::endl;
			if (it->type == Else)
			{
				//  All Else tokens should've been
				std::stringstream ss;
				ss << "Unexpected else statement in \""
				   << it->line
				   << "\" on line #"
				   << it->lineNumber;
				throw SyntaxErrorException(ss.str());
			}
			//it->statement = it->createStatement();
			statements.push_back(it->createStatement());
		}
		//std::cout << "}"; std::cout.flush();
	}
	catch (const SyntaxErrorException& see)
	{
		//  Free all the dynamically allocated memory in the node-chain before throwing
		for (SNode *it = head, *temp = nullptr; it != nullptr; )
		{
			if (it->statement)
			{
				delete it->statement;
			}
			if (it->expression)
			{
				delete it->expression;
			}
			temp = it;
			it = it->right;
			delete temp;
		}
		throw see;
	}
	Statement *retVal = new StatementBlock(statements);
	return retVal;
}

char tokenToChar(TokenType type)
{
	switch (type)
	{
		case If:
			return 'i';
		case Else:
			return 'e';
		case While:
			return 'w';
		case SetVar:
			return 'v';
		case StringSet:
			return 's';
		case ProcedureCall:
			return 'p';
		case OpenBrace:
			return '{';
		case CloseBrace:
			return '}';
		default:
			return '?';
	}
}

}
