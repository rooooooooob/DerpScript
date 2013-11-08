/**
 * @section DESCRIPTION
 * RAII wrapper around pushing the stack. pops stack on dtor
 */
#ifndef DS_STACKPUSH_HPP
#define DS_STACKPUSH_HPP

#include "ParameterList.hpp"

namespace ds
{

class StackPush
{
public:
	StackPush(Context& context, const ParameterList& parameters, const std::vector<std::string>& parameterNames,
		const std::vector<ParameterList::Type>& parameterTypes);

	~StackPush();

private:
	StackPush(const StackPush& othre) {}	//	disable
	StackPush& operator=(const StackPush& rhs) {}	//	disable
};

}

#endif