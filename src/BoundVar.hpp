#ifndef DS_BOUNDVAR_HPP
#define DS_BOUNDVAR_HPP

namespace ds
{

class BoundVar
{
public:
	virtual ~BoundVar() {};

	virtual void set(float value) = 0;

	virtual float get() const = 0;

protected:
	BoundVar() {}
};

}

#endif
