#include "SyntaxErrorException.hpp"

namespace ds
{

SyntaxErrorException::SyntaxErrorException(const std::string& info)
	:info("Syntax error: ")
{
	this->info += info;
}

SyntaxErrorException::SyntaxErrorException(const SyntaxErrorException& base, const std::string& info)
	:info(base.info)
{
	this->info += info;
}

const char * SyntaxErrorException::what() const throw()
{
	return info.c_str();
}

}
