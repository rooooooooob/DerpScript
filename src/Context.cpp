#include "Context.hpp"

#include <utility>
#include <iostream>
#include <sstream>
#include "RuntimeInterpreterErrorException.hpp"

namespace ds
{

Context::Context()
	:returnValue(1337)
{
	localVars.push(new FlagDB());
}

Context::~Context()
{
	eraseEverything();
}

void Context::setVar(const std::string& scope, const std::string& name, float value)
{
	try
	{
		variables.at(scope).at(name)->set(value);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error encountered setting " << scope << "." << name << ":\n\t" << e.what() << std::endl;
		std::cerr << e.what();
	}
}

float Context::getVar(const std::string& scope, const std::string& name) const
{
	float value;
	try
	{
		value = variables.at(scope).at(name)->get();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error encountered getting " << scope << "." << name << ":\n\t" << e.what() << std::endl;
		//oh no, enjoy your zero (at least for now)
		value = 0;
	}
	return value;
}

void Context::setFlag(const std::string& scope, const std::string& name, float value)
{
	if (scope == "local")
	{
		localVars.top()->set(name, value);
	}
	else
	{
		if (flags.count(scope) == 0)
		{
			flags[scope] = new FlagDB();
		}
		flags[scope]->set(name, value);
	}
}

float Context::getFlag(const std::string& scope, const std::string& name) const
{
	float value;
	try
	{
		if (scope == "local")
		{
			value = localVars.top()->get(name);
		}
		else
		{
			value = flags.at(scope)->get(name);
		}
	}
	catch (const std::exception& e)
	{
		value = 0;
	}
	return value;
}

bool Context::variableExists(const std::string& scope, const std::string& name) const
{
	//std::unordered_map<std::string, std::unordered_map<std::string, BoundVar*> >::iterator scopeIterator = variables.find(scope);
	auto scopeIterator = variables.find(scope);
	if (scopeIterator != variables.end())   //  scope exists
	{
		//std::unordered_map<std::string, BoundVar*>::iterator nameIterator = scopeIterator->second.find(name);
		auto nameIterator = scopeIterator->second.find(name);
		return (nameIterator != scopeIterator->second.end());   //  return if name exists in scope
	}
	return false;
}

void Context::pushStack()
{
	localVars.push(new FlagDB());
	localStrings.push(std::map<std::string, std::string>());
}

void Context::popStack()
{
	if (!localVars.empty())
	{
		delete localVars.top();
		localVars.pop();
	}
	if (!localStrings.empty())
	{
		localStrings.pop();
	}
}

void Context::registerVar(const std::string& scope, const std::string& name, BoundVar* var)
{
	std::unordered_map<std::string, std::unordered_map<std::string, BoundVar*> >::iterator scopeIterator = variables.find(scope);
	if (scopeIterator != variables.end())   //  scope exists
	{
		std::unordered_map<std::string, BoundVar*>::iterator nameIterator = scopeIterator->second.find(name);
		if (nameIterator != scopeIterator->second.end())   //  variable also exists
		{
			//  maybe report some kind of warning/error/something?
			std::cerr << "Redefinition of [" << scope << ":" << name << "]" << std::endl;
			delete nameIterator->second;
			nameIterator->second = var;
		}
		else	//  Add variable, scope already exists
		{
			scopeIterator->second.insert(std::make_pair(name, var));
		}
	}
	else	//  Scope doesn't even exist, add both.
	{
		variables[scope][name] = var;
	}
}

void Context::registerString(const std::string& scope, const std::string& name, BoundString *string)
{
	std::map<std::string, std::map<std::string, BoundString*> >::iterator scopeIterator = strings.find(scope);
	if (scopeIterator != strings.end())   //  scope exists
	{
		std::map<std::string, BoundString*>::iterator nameIterator = scopeIterator->second.find(name);
		if (nameIterator != scopeIterator->second.end())   //  variable also exists
		{
			//  maybe report some kind of warning/error/something?
			std::cerr << "Redefinition of [" << scope << ":" << name << "]" << std::endl;
			delete nameIterator->second;
			nameIterator->second = string;
		}
		else	//  Add variable, scope already exists
		{
			scopeIterator->second.insert(std::make_pair(name, string));
		}
	}
	else	//  Scope doesn't even exist, add both.
	{
		strings[scope][name] = string;
	}
}

void Context::registerFlagDB(const std::string& scope, FlagDB *database)
{
	if (flags[scope])
	{
		std::cerr << "Warning: Overwriting old FlagDB at scope <" << scope << ">" << std::endl;
		delete flags[scope];
	}
	flags[scope] = database;
}

void Context::registerFunction(const std::string& scope, const std::string& name, const std::string& params, const std::function<float(const ParameterList&)>& function)
{
	generateRegisterMethod(scope, name, params, function, numericalFunctions, "number");
}

void Context::registerStringFunction(const std::string& scope, const std::string& name, const std::string& params, const std::function<std::string(const ParameterList&)>& function)
{
	generateRegisterMethod(scope, name, params, function, stringFunctions, "string");
}

void Context::registerProcedure(const std::string& scope, const std::string& name, const std::string& params, const std::function<void(const ParameterList&)>& function)
{
	generateRegisterMethod(scope, name, params, function, procedures, "void");
}

float Context::evaluateNumericalFunction(const std::string& scope, const std::string& name, const ParameterList& parameters) const
{
	//std::cout << "evaling " << scope << ":" << name << parameters.getFormattedSignature() << std::endl;
	return generateEvaluateMethod<float, decltype(numericalFunctions)>(scope, name, parameters, numericalFunctions, "number");
}

std::string Context::evaluateStringFunction(const std::string& scope, const std::string& name, const ParameterList& parameters) const
{
	return generateEvaluateMethod<std::string, decltype(stringFunctions)>(scope, name, parameters, stringFunctions, "string");
}

void Context::executeProcedure(const std::string& scope, const std::string& name, const ParameterList& parameters)
{
	generateEvaluateMethod<void, decltype(procedures)>(scope, name, parameters, procedures, "void");
}

void Context::eraseEverything()
{
	for (std::unordered_map<std::string, std::unordered_map<std::string, BoundVar*> >::iterator it = variables.begin(); it != variables.end(); ++it)
	{
		for (std::unordered_map<std::string, BoundVar*>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			delete it2->second;
		}
		it->second.clear();
	}
	variables.clear();
	for (std::map<std::string, std::map<std::string, BoundString*> >::iterator it = strings.begin(); it != strings.end(); ++it)
	{
		for (std::map<std::string, BoundString*>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			delete it2->second;
		}
		it->second.clear();
	}
	strings.clear();

	while (!localVars.empty())
	{
		delete localVars.top();
		localVars.pop();
	}
}

void Context::unregisterScope(const std::string& scope)
{
	std::unordered_map<std::string, std::unordered_map<std::string, BoundVar*> >::iterator scopeIterator = variables.find(scope);
	if (scopeIterator != variables.end())
	{
		for (std::unordered_map<std::string, BoundVar*>::iterator it = scopeIterator->second.begin(); it != scopeIterator->second.end(); ++it)
		{
			delete it->second;
		}
		scopeIterator->second.clear();
	}
	else
	{
		std::cerr << "Trying to unregister scope [" << scope << ":*] but scope doesn't even exist in the first place. :(" << std::endl;
	}
}

void Context::unregisterVar(const std::string& scope, const std::string& name)
{
	std::unordered_map<std::string, std::unordered_map<std::string, BoundVar*> >::iterator scopeIterator = variables.find(scope);
	if (scopeIterator != variables.end())   //  scope exists
	{
		std::unordered_map<std::string, BoundVar*>::iterator nameIterator = scopeIterator->second.find(name);
		if (nameIterator != scopeIterator->second.end())   //  variable also exists
		{
			delete nameIterator->second;
			scopeIterator->second.erase(nameIterator);
		}
		else
		{
			std::cerr << "Trying to unregister var [" << scope << ":" << name << "] but var wasn't registered in the first place." << std::endl;
		}
	}
	else
	{
		std::cerr << "Trying to unregister var [" << scope << ":" << name << "] but scope doesn't even exist in the first place. :(" << std::endl;
	}
}

const std::string Context::getString(const std::string& scope, const std::string& name) const
{
	std::string value;
	try
	{
		value = strings.at(scope).at(name)->get();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error encountered getting " << scope << "." << name << ":\n\t" << e.what() << std::endl;
		value = "<error: " + scope + "." + name + " doesn't exist>";
	}
	return value;
}

void Context::setString(const std::string& scope, const std::string& name, const std::string& value)
{
	try
	{
		strings.at(scope).at(name)->set(value);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error encountered setting " << scope << "." << name << ":\n\t" << e.what() << std::endl;
		std::cerr << e.what();
	}
}

const std::string Context::getStringFlag(const std::string& scope, const std::string& name) const
{
	std::string value;
	try
	{
		if (scope == "local")
		{
			value = localStrings.top().at(name);
		}
		else
		{
			value = stringDBs.at(scope)->get(name);
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error encountered getting " << scope << ":" << name << ":\n\t" << e.what() << std::endl;
		value = "<error: " + scope + ":" + name + " doesn't exist>";
	}
	return value;
}

void Context::setStringFlag(const std::string& scope, const std::string& name, const std::string& value)
{
	if (scope == "local")
	{
		localStrings.top()[name] = value;
	}
	else
	{
		 if (stringDBs.count(scope) == 0)
		 {
			 stringDBs[scope] = new StringDB();
		 }
		 stringDBs[scope]->set(name, value);
	}
}

float Context::getReturnValue() const
{
	return returnValue;
}

void Context::setReturnValue(float value)
{
	returnValue = value;
}

#ifdef DS_DEBUG
void Context::print()
{
	std::cout << "Printing bound context vars:" << std::endl;
	for (std::unordered_map<std::string, std::unordered_map<std::string, BoundVar*> >::iterator it = variables.begin(); it != variables.end(); ++it)
	{
		for (std::unordered_map<std::string, BoundVar*>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			std::cout << "\t" << it->first << "." << it2->first << " = " << it2->second->get() << std::endl;
		}
	}
}
#endif

}
