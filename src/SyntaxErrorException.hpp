#ifndef DS_SYNTAXERROREXCEPTION_HPP
#define DS_SYNTAXERROREXCEPTION_HPP

#include <exception>
#include <string>

namespace ds
{

class SyntaxErrorException : public std::exception
{
public:
	SyntaxErrorException(const std::string& info);
	SyntaxErrorException(const SyntaxErrorException& base, const std::string& info);//ammends info to base
	virtual ~SyntaxErrorException() throw() {}

	virtual const char * what() const throw();
private:
	std::string info;
};

}

#endif
