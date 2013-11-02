#include "StringDB.hpp"

namespace ds
{

StringDB::StringDB()
{
}

void StringDB::set(const std::string& name, const std::string& value)
{
	strings[name] = value;
}

const std::string& StringDB::get(const std::string& name) const
{
	std::string value;
	try
	{
		value = strings.at(name);
	}
	catch (const std::exception& e)
	{
		value = "";
	}
	return value;
}

}
