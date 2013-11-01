#ifndef DS_BOUNDFLOAT_HPP
#define DS_BOUNDFLOAT_HPP

#include "BoundVar.hpp"

namespace ds
{

class BoundFloat : public BoundVar
{
public:
	BoundFloat(float& reference);

	void set(float value);

	float get() const;
private:
	float& variable;
};

}

#endif
