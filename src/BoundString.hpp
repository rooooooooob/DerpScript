#ifndef DS_BOUNDSTRING_HPP
#define DS_BOUNDSTRING_HPP

#include <string>

namespace ds
{

class BoundString
{
public:
	BoundString(std::string& reference);

	void set(const std::string& value);

	const std::string& get() const;
private:
	std::string& string;
};

}

#endif
