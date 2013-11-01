#include "FlagDB.hpp"

namespace ds
{

FlagDB::FlagDB()
{
}

void FlagDB::set(const std::string& name, int value)
{
	flags[name] = value;
}

int FlagDB::get(const std::string& name) const
{
	int value;
	try
	{
		value = flags.at(name);
	}
	catch (const std::exception& e)
	{
		value = 0;
	}
	return value;
}

}
