#include "RuntimeInterpreterErrorException.hpp"

namespace ds
{

RuntimeInterpreterErrorException::RuntimeInterpreterErrorException(const std::string& info)
	:info("Runtime interpreter error: ")
{
	this->info += info;
}

const char * RuntimeInterpreterErrorException::what() const throw()
{
	return info.c_str();
}

}
