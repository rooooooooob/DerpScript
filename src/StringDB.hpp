#ifndef DS_STRINGDB_HPP
#define DS_STRINGDB_HPP

#include <string>
#include <unordered_map>
//#include "Core/Save.hpp"

namespace ds
{

class StringDB
{
public:
	StringDB();

	void set(const std::string& name, const std::string& value);

	std::string get(const std::string& name) const;

	//void save(Save& save) const;

private:
	std::unordered_map<std::string, std::string> strings;
};

}

#endif
