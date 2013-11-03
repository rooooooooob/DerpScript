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

std::string StringDB::get(const std::string& name) const
{
	std::string value;
	auto it = strings.find(name);
	if (it == strings.end())
	{
		value = "";
	}
	else
	{
		value = it->second;
	}
	return value;
}

}
