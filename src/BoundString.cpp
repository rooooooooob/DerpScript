#include "BoundString.hpp"

namespace ds
{

BoundString::BoundString(std::string& reference)
	:string(reference)
{
}

void BoundString::set(const std::string& value)
{
	string = value;
}

const std::string& BoundString::get() const
{
	return string;
}

}
