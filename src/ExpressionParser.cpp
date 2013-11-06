#include "ExpressionParser.hpp"

#include <vector>
#include <cstdlib>
#include <initializer_list>
#include <memory>
#include "Expressions/Variable.hpp"
#include "Expressions/Flag.hpp"
#include "Expressions/Constant.hpp"
#include "Expressions/ArithmeticOperators.hpp"
#include "Expressions/LogicalOperators.hpp"
#include "Expressions/RelationalOperators.hpp"
#include "SyntaxErrorException.hpp"
#include "StringParser.hpp"
#include "Expressions/Function.hpp"
#include "FunctionParameterParser.hpp"
#include "ParameterList.hpp"


namespace ds
{

/*			  I apologize in advance for this. I didn't know anything				 */
/*			  about formal grammars or anything before writing this.				  */
/*			  I am very, very sorry for how ugly and long this file this			  */
/*			  file is. So hopefully no one ever has to look in here. It's			 */
/*			  very C-style, but at least the extern interface is simple!			  */
/*			  Not that C is bad or anything.										  */
/*			  But hey, it works, right? God I hope it works...						*/
/*			  And I also really hope throw exceptions doesn't leak memory			 */
/*			  like there's no tomorrow...											 */


///////////////////////////////////////////////////////////////////
//		  DECLARATION OF STUFF USED ONLY IN THIS FILE		   //
///////////////////////////////////////////////////////////////////

enum TokenType
{
	Add,	//	addition
	Sub,	//	subtration
	Mul,	//	multiplication
	Div,	//	division
	Mod,	//	modulo
	Var,	//	variable (float)
	Num,	//	constant float literal
	RBr,	//	right bracket
	LBr,	//	left bracket
	Inv,	//	logical not
	UMi,	//	unary minus
	Gre,	//	greater than
	Les,	//	less than
	GrE,	//	greater than or equal
	LeE,	//	less than or equal
	Eq,		//	equality
	NEq,	//	not equal
	And,	//	logical and
	Or,		//	logical or
	Fun		//	number-returning function call
};

struct EToken
{
	EToken(const TokenType type);


	const TokenType type;

	Expression *unary;//in the case of constants, vars, and flags
	const ParameterList *params;
};


struct ENode
{
	ENode(const EToken type, ENode*& head);

	Expression* createExpression();
	void killLeft();
	void killRight();


	const TokenType type;
	Expression *exp;
	ENode *left, *right;
	ENode*& head;
};

///////////////////////////////////////////////////
//		  static function prototypes		   //
///////////////////////////////////////////////////


//  This parser is such a mess...

static void createTokens(std::vector<EToken>& tokens, const char *exp);

static Expression *parseTokens(const std::vector<EToken>& tokens, std::size_t start, std::size_t end);

static void parseLevel(std::initializer_list<TokenType> il, ENode*& head);


static bool isBinaryOperator(TokenType type);

static bool isUnaryOperator(TokenType type);

static bool isOperand(TokenType type);

static char tokenToChar(TokenType type);

////////////////////////////////////////////////////
//		  PUBLIC FUNCTION DEFINITIONS		   //	<--------EXTERN-DEFINITIONS-HERE---------<<<
////////////////////////////////////////////////////



Expression* parseExpression(const char *exp)
{
	std::vector<EToken> tokens;

	Expression *retVal = nullptr;

	createTokens(tokens, exp);

	retVal = parseTokens(tokens, 0, tokens.size());

	if (!retVal)
	{
		//  This shouldn't happen. I don't think it can, actually.
		retVal = new Constant(0);
	}

	return retVal;
}



//////////////////////////////////////////////////
//		  struct method definitions		   //	  <-----NOW-MORE-LOCAL-STUFF-----<<<
//////////////////////////////////////////////////

EToken::EToken(TokenType type)
	:type(type)
	,unary(nullptr)
	,params(nullptr)
{
}

ENode::ENode(EToken type, ENode*& head)
	:type(type.type)
	,exp(type.unary)
	,left(nullptr)
	,right(nullptr)
	,head(head)
{
}

Expression* ENode::createExpression()
{
	if (!exp)
	{
		if (isBinaryOperator(type))
		{
			// do not uncomment. make proper errors instead, as 1 + 2 + 3 breaks this.
			/*if ((!isOperand(left->type) && !isUnaryOperator(left->type)) || (!isOperand(right->type) && !isUnaryOperator(right->type)))
			{
				std::stringstream ss;
				ss << "Binary operator encountered without an operand on each side: '"
				   << tokenToChar(left->type)
				   << ' '
				   << tokenToChar(type)
				   << ' '
				   << tokenToChar(right->type)
				   << "'\n";
				throw SyntaxErrorException(ss.str());
			}
			if (isUnaryOperator(type) && (!isUnaryOperator(right->type) && !isOperand(right->type)))
			{
				std::stringstream ss;
				ss << "Unary operator encountered without an operand to the right: '"
				   << tokenToChar(type)
				   << tokenToChar(right->type)
				   << "'\n";
				throw SyntaxErrorException(ss.str());
			}*/
		}
		switch (type)
		{
			case Add:
				exp = new Addition(left->createExpression(), right->createExpression());
				break;
			case Sub:
				exp = new Subtraction(left->createExpression(), right->createExpression());
				break;
			case Mul:
				exp = new Multiplication(left->createExpression(), right->createExpression());
				break;
			case Div:
				exp = new Division(left->createExpression(), right->createExpression());
				break;
			case Mod:
				exp = new Modulo(left->createExpression(), right->createExpression());
				break;
			case Gre:
				exp = new GreaterThan(left->createExpression(), right->createExpression());
				break;
			case Les:
				exp = new LessThan(left->createExpression(), right->createExpression());
				break;
			case GrE:
				exp = new GreaterThanOrEqual(left->createExpression(), right->createExpression());
				break;
			case LeE:
				exp = new LessThanOrEqual(left->createExpression(), right->createExpression());
				break;
			case Eq:
				exp = new Equals(left->createExpression(), right->createExpression());
				break;
			case NEq:
				exp = new Inverse(new Equals(left->createExpression(), right->createExpression()));
				break;
			case And:
				exp = new LogicalAnd(left->createExpression(), right->createExpression());
				break;
			case Or:
				exp = new LogicalOr(left->createExpression(), right->createExpression());
				break;
			case Inv:
				exp = new Inverse(right->createExpression());
				break;
			case UMi:
				exp = new UnaryMinus(right->createExpression());
				break;
			default:
				break;
		}
		if (isBinaryOperator(type))
		{
			killLeft();
			killRight();
		}
		else if (isUnaryOperator(type))
		{
			killRight();
		}
	}
	Expression *retVal = exp;
	exp = nullptr; //  transfer ownership
	return retVal;
}

void ENode::killLeft()
{
	if (!left)
	{
		return;
	}
	if (left == head)
	{
		head = this;
	}
	ENode *temp = left;
	if (left->left)
	{
		left->left->right = this;
	}
	left = left->left;

	if (temp->exp)
	{
		delete temp->exp;
	}
	delete temp;
}

void ENode::killRight()
{
	if (!right)
	{
		return;
	}
	ENode *temp = right;
	if (right->right)
	{
		right->right->left = this;
	}
	right = right->right;

	if (temp->exp)
	{
		delete temp->exp;
	}
	delete temp;
}


/*		  END OF TOKEN METHODS			*/


////////////////////////////////////////////////////
//		  STATIC FUNCTION DEFINITIONS		   //
////////////////////////////////////////////////////


void createTokens(std::vector<EToken>& tokens, const char *exp)
{
	tokens.clear(); //  memory leaks lol
	bool singleChar; //  whether or not a single-char token was parsed on a given iteration
	for (const char *c = exp; *c != '\0' && *c != '@' && !(*c == '/' && *(c + 1) == '/'); )
	{
		singleChar = true;
		if (*c == '+')							  //  Arithmetic operators
		{
			tokens.push_back(Add);
		}
		else if (*c == '-')
		{
			if (tokens.empty() || !isOperand(tokens.back().type))
			{
				tokens.push_back(UMi);
			}
			else
			{
				tokens.push_back(Sub);
			}
		}
		else if (*c == '*')
		{
			tokens.push_back(Mul);
		}
		else if (*c == '/')
		{
			tokens.push_back(Div);
		}
		else if (*c == '%')
		{
			tokens.push_back(Mod);
		}
		else if (*c == '=')					 //  Realational operators
		{
			while (*(c + 1) == '=')
			{
				++c;
			}
			tokens.push_back(Eq);
		}
		else if (*c == '>')
		{
			if (*(c + 1) == '=')
			{
				tokens.push_back(GrE);
				++c;
			}
			else
			{
				tokens.push_back(Gre);
			}
		}
		else if (*c == '<')
		{
			if (*(c + 1) == '=')
			{
				tokens.push_back(LeE);
				++c;
			}
			else
			{
				tokens.push_back(Les);
			}
		}
		else if (*c == '!')
		{
			if (*(c + 1) == '=')
			{
				while (*(c + 1) == '=')
				{
					++c;
				}
				tokens.push_back(NEq);
			}
			else								//  Logical operators
			{
				tokens.push_back(Inv);
			}
		}
		else if (*c == 'o' && *(c + 1) == 'r')
		{
			tokens.push_back(Or);
			++c;
		}
		else if (*c == '|')
		{
			while (*(c + 1) == '|')
			{
				++c;
			}
			tokens.push_back(Or);
		}
		else if (*c == 'a' && *(c + 1) == 'n' && *(c + 2) == 'd')
		{
			tokens.push_back(And);
			c += 2;
		}
		else if (*c == '&')
		{
			while (*(c + 1) == '&')
			{
				++c;
			}
			tokens.push_back(And);
		}
		else if (*c == '(')				 //  Fucking parenthesis
		{
			tokens.push_back(LBr);
		}
		else if (*c == ')')
		{
			tokens.push_back(RBr);
		}
		else if (*c == 't' && *(c + 1) == 'r' && *(c + 2) == 'u' && *(c + 3) == 'e')
		{
			tokens.push_back(Num);
			tokens.back().unary = new Constant(1);
			c += 3;
		}
		else if (*c == 'f' && *(c + 1) == 'a' && *(c + 2) == 'l' && *(c + 4) == 'l' && *(c + 5) == 'e')
		{
			tokens.push_back(Num);
			tokens.back().unary = new Constant(0);
			c += 4;
		}
		else if ((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z'))  //  variables / flags / functions
		{
			singleChar = false;
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
			if (onScope)	//  Must be a local var then
			{
				name = scope;
				scope = "local";
				isFlag = true;
			}

			if (*c == '(')	// might be a function
			{
				tokens.push_back(Fun);
				int depthLevel = 0;
				const char *startOfParam = c + 1;
				while (*c != '\0')
				{
					if (*c == '(')
					{
						++depthLevel;
					}
					else if (*c == ')')
					{
						if (--depthLevel <= 0)
						{
							std::string buffer(startOfParam, c - startOfParam);
							//std::cout << "arglist of " << scope << "." << name << "() is [" << buffer << "]" << std::endl;
							tokens.back().unary = new Function(scope, name, parseFunctionParameters(buffer.c_str()));
							break;
						}
					}
					++c;
				}
			}
			else
			{
				tokens.push_back(Var);
				if (isFlag)
				{
					tokens.back().unary = new Flag(scope, name);
				}
				else
				{
					tokens.back().unary = new Variable(scope, name);
				}
			}
		}
		else if (*c >= '0' && *c <= '9')		//  Numerical constants
		{
			singleChar = false;
			tokens.push_back(Num);
			std::string number;
			while ((*c >= '0' && *c <= '9') || *c == '.')
			{
				number += *c;
				++c;
			}
			tokens.back().unary = new Constant(atof(number.c_str()));
		}
		if (singleChar)
		{
			++c;
		}
	}
}

Expression *parseTokens(const std::vector<EToken>& tokens, std::size_t start, std::size_t end)
{
	ENode *head = nullptr, *cur = nullptr, *temp, *tail = nullptr;
	try
	{
		//handle parenthesis fuckery
		for (std::size_t i = start; i < end; ++i)
		{
			if (tokens[i].type == RBr)
			{
				continue;
			}
			temp = new ENode(tokens[i], head);
			if (!head)
			{
				head = temp;
			}
			if (cur)
			{
				cur->right = temp;
				temp->left = cur;
			}
			cur = tail = temp;

			if (tokens[i].type == LBr)
			{
				std::size_t newStart = i + 1;
				int depth = 0;
				while (i < end)
				{
					if (tokens[i].type == LBr)
					{
						++depth;
					}
					else if (tokens[i].type == RBr)
					{
						--depth;
						if (depth == 0)
						{
							break;
						}

					}
					++i;
				}
				//skip ahead until Rbr then call this on that
				cur->exp = parseTokens(tokens, newStart, i);

			}
		}

		if (!head)
		{
			return new Constant(0);
		}

		//  Iterate right-to-left to parse unary ops
		for (ENode *it = tail; it != nullptr; it = it->left)
		{
			if (isUnaryOperator(it->type))
			{
				it->exp = it->createExpression();
			}
		}


		parseLevel({Mul, Div, Mod}, head);
		parseLevel({Add, Sub}, head);
		parseLevel({Gre, Les, GrE, LeE}, head);
		parseLevel({Eq, NEq}, head);
		parseLevel({And}, head);
		parseLevel({Or}, head);
	}
	catch (const SyntaxErrorException& see)
	{
		//  Free all the dynamically allocated memory in the node-chain before throwing
		for (ENode *it = head, *temp = nullptr; it != nullptr; )
		{
			if (it->exp)
			{
				delete it->exp;
			}
			temp = it;
			it = it->right;
			delete temp;
		}
		throw see;
	}

	return head->createExpression();
}

void parseLevel(std::initializer_list<TokenType> il, ENode*& head)
{
	ENode *it = head;

	while (it != nullptr)
	{
		for (auto tokType = il.begin(); tokType != il.end(); ++tokType)
		{
			if (it->type == *tokType)
			{
				it->exp = it->createExpression();
				break;  //  Already matched one type, no need to check rest
			}
		}
		it = it->right;
	}
}

bool isBinaryOperator(TokenType type)
{
	return (!isUnaryOperator(type) && !isOperand(type));
}
bool isUnaryOperator(TokenType type)
{
	return (type == Inv || type == UMi);
}
bool isOperand(TokenType type)
{
	return (type == Var || type == Num || type == RBr || type == LBr || type == Fun);
}

char tokenToChar(TokenType type)
{
	switch (type)
	{
		case Add:
			return '+';
		case Sub:
			return '-';
		case Mul:
			return '*';
		case Div:
			return '/';
		case Mod:
			return '%';
		case Var:
			return 'V';
		case Num:
			return 'C';
		case RBr:
			return ')';
		case LBr:
			return '(';
		case Inv:
			return '!';
		case UMi:
			return 'm';
		case Gre:
			return '>';
		case Les:
			return '<';
		case GrE:
			return 'g';
		case LeE:
			return 'l';
		case Eq:
			return '=';
		case NEq:
			return 'n';
		case And:
			return '&';
		case Or:
			return '|';
		default:
			return '?';
	}
}

}
