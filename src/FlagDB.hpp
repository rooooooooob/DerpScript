#ifndef DS_FLAGDB_HPP
#define DS_FLAGDB_HPP

#include <string>
#include <unordered_map>
//#include "Core/Save.hpp"

namespace ds
{

class FlagDB
{
public:
	FlagDB();

	void set(const std::string& name, float value);

	float get(const std::string& name) const;

	//void save(Save& save) const;

private:
	std::unordered_map<std::string, float> flags;
};

}

#endif
