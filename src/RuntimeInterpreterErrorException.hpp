#ifndef DS_RUNTIMEINTERPRETERERROREXCEPTION_HPP
#define DS_RUNTIMEINTERPRETERERROREXCEPTION_HPP

#include <exception>
#include <string>

namespace ds
{

class RuntimeInterpreterErrorException : public std::exception
{
public:
	RuntimeInterpreterErrorException(const std::string& info);
	
	virtual ~RuntimeInterpreterErrorException() throw() {}

	virtual const char * what() const throw();
private:
	std::string info;
};

}

#endif