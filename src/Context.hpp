/**
 * @section DESCRIPTION
 * This class gives a context to any syntax tree structures (Expressions, Statements) since
 * otherwise they're context-insensitive. Basically, this tells them what kind of functions,
 * variables and flags it has access to as well as storing those things. You can bind variables
 * functions and other stuff to this class so that you can call upon them via the interpreted language
 */

#ifndef DS_CONTEXT_HPP
#define DS_CONTEXT_HPP

#include <string>
#include <unordered_map>
#include <map>
#include <sstream>
#include <iostream>
#include <stack>
#include <vector>
#include <functional>
#include "BoundVar.hpp"
#include "BoundString.hpp"
#include "FlagDB.hpp"
#include "ParameterList.hpp"
#include "RuntimeInterpreterErrorException.hpp"

namespace ds
{

class Context
{
public:
	/**
	 * Constructs a Context with nothing bound to it yet.
	 */
	Context();
	/**
	 * See: Context::deleteEverything() for more info
	 */
	~Context();
	/**
	 * Sets a variable's value.
	 * If the variable doesn't exist, an error is generated unless it's in the
	 * scope 'local' in which case the variable is created and set as well.
	 * @param scope The scope that the varible resides in
	 * @param name The name of the variable
	 * @param value The value to set the variable as
	 */
	void setVar(const std::string& scope, const std::string& name, float value);
	/**
	 * Gets the value of a variable stored in this context.
	 * An error is generated if the variable doesn't exist.
	 * @param scope The scope that the variable resides in
	 * @param name The name of the variable
	 * @return The value of the variable
	 */
	float getVar(const std::string& scope, const std::string& name) const;
	/**
	 * Sets a flag's value. If no such flag exists (but scope does), the variable
	 * is created in that scope and then set.
	 * However, if the scope doesn't exist then an error is generated.
	 * @param scope The scope in which the flag is in.
	 * @param name The name of the flag
	 * @param value The value to set the flag to
	 */
	void setFlag(const std::string& scope, const std::string& name, float value);
	/**
	 * Gets the value of a flag stored in this context.
	 * If the context doesn't exist then an error is generated.
	 * If the context exists but the flag doesn't 0 is returned.
	 * @param scope The scope the flag resides in (which FlagDB)
	 * @param name The name of the flag
	 * @return The value of the flag (0 if flag doesn't exist)
	 */
	float getFlag(const std::string& scope, const std::string& name) const;
	/**
	 * Checks to see if there is a variable registered to the input scope/name
	 * @param scope The scope to check in
	 * @param name The name to check for
	 * @return True if the variable exists, false otherwise.
	 */
	bool variableExists(const std::string& scope, const std::string& name) const;
	/**
	 * Pushes a new map for local variables on. This is to be called at the start of funtions.
	 */
	void pushStack();
	/**
	 * Pops the top map for local variables. This is to be called at the end of functions.
	 */
	void popStack();
	/**
	 * Binds a C++ variable/something that acts like one to this Context's variable lists
	 * so that it can be accessed from within the scripting language. If a variable is
	 * already bound to the name/scope it is deleted and replaced.
	 * @param scope The scope to beind it to
	 * @param name The name to bind it to
	 * @param var The variable to bind
	 */
	void registerVar(const std::string& scope, const std::string& name, BoundVar* var);

	void registerString(const std::string& scope, const std::string& name, BoundString *string);

	/**
	 * Registers a FlagDB to the given scope so that it can be accessed (read/write) from interpreted code
	 * Note: NO OWNERSHIP IS ASSUMED WHATSOEVER BY CONTEXT OVER THE FLAGDB
	 * @param scope The scope to register the FlagDB to
	 * @param database The FlagDB to register.
	 */
	void registerFlagDB(const std::string& scope, FlagDB *database);

	void registerFunction(const std::string& scope, const std::string& name, const std::string& params, const std::function<float(const ParameterList&)>& function);

	void registerFunction(const std::string& scope, const std::string& name, const std::string& params, const std::function<std::string(const ParameterList&)>& function);

	void registerProcedure(const std::string& scope, const std::string& name, const std::string& params, const std::function<void(const ParameterList&)>& function);

	float evaluateNumericalFunction(const std::string& scope, const std::string& name, const ParameterList& parameters) const;

	std::string evaluateStringFunction(const std::string& scope, const std::string& name, const ParameterList& parameters) const;

	void executeProcedure(const std::string& scope, const std::string& name, const ParameterList& parameters);

	/**
	 * Deletes all BoundVar and BoundString structures and unregisters everything else
	 * (FlagDBs registered however are not deleted since they are typically owned by
	 * other classes! - with the exception of the local/temp/default one)
	 */
	void eraseEverything();
	/**
	 * Unregisters all varibles / flags / functions in a given scope.
	 * @param scope The scope to unregister, if it existed
	 */
	void unregisterScope(const std::string& scope);
	/**
	 * Unregisters a certain variable in a given scope
	 * @param scope The scope the variable is in
	 * @param name The name of the variable
	 */
	void unregisterVar(const std::string& scope, const std::string& name);

	const std::string getString(const std::string& scope, const std::string& name) const;

	void setString(const std::string& scope, const std::string& name, const std::string& value);

#ifdef DS_DEBUG
	/**
	 * Debug print method that prints out all variables/flags/functions bound/stored in this context.
	 */
	void print();
#endif

private:
	//!	A lookup table of all variables bound to this Context
	std::unordered_map<std::string, std::unordered_map<std::string, BoundVar*> > variables;
	//!	A lookup table of all Flag Databases bound to this Context
	std::unordered_map<std::string, FlagDB*> flags;
	//!	A map of all string-returning functions bound to this Context
	std::map<std::string, std::map<std::string, std::map<std::string, std::function<std::string(const ParameterList&)> > > > stringFunctions;
	//!	A map of all numerical-returning functions bound to this Context
	std::map<std::string, std::map<std::string, std::map<std::string, std::function<float(const ParameterList&)> > > > numericalFunctions;
	//!	A map of all non-returning procedures bound to this Context
	std::unordered_map<std::string, std::map<std::string, std::map<std::string, std::function<void(const ParameterList&)> > > > procedures;
	//std::stack<std::vector<std::string> > stackVars;
	//!	The local variables currently on the stack. Only the ones on the top are accessable.
	std::stack<std::map<std::string, float> > localVars;

	std::map<std::string, std::map<std::string, BoundString*> > strings;
};

/*
	The following functions are used to generate some of the methods in Context's logic
	since the logic in the functions was nearly identical but due to types and stuff they
	couldn't be replaced without using templates.
*/

/**
 * Generates a function to register functions. This is templated to not necessitate 3+ goddamned
 * other registerFunction/Procedure bodies of nearly identical code.
 * T	=	type of function, ie std::function<float(const ParameterList&)>
 * MT	=	type of functionMap, ie map of maps of maps of Ts
 * @param scope The scope to register the function to (key for first map)
 * @param name The name to register in the scope (key for second map)
 * @param params The parameter signature of the function to register (key for last map)
 * @param function The actual function to register
 * @param functionMap The map of maps of maps to register the function to
 * @param typeName String saying function's return type (used in error message)
 */
template <typename T, typename MT>
static void generateRegisterMethod(const std::string& scope, const std::string& name, const std::string& params, const T& function, MT& functionMap, const std::string& typeName)
{
	auto scopeIterator = functionMap.find(scope);
	if (scopeIterator != functionMap.end())   //  scope exists
	{
		auto nameIterator = scopeIterator->second.find(name);
		if (nameIterator != scopeIterator->second.end())   //  variable also exists
		{
			auto paramIterator = nameIterator->second.find(params);
			if (paramIterator != nameIterator->second.end())	//	function with same signature exists already
			{
				//  maybe report some kind of warning/error/something?
				std::cerr << "Redefinition of [" << scope << ":" << name << "(" << params << ") -> " << typeName << "]" << std::endl;
				paramIterator->second = function;
			}
			else
			{
				nameIterator->second.insert(std::make_pair(params, function));
			}
		}
		else	//  Add variable + params, scope already exists
		{
			//	I am going to hell for this line...
			//	(insert a string->function map into the name's map, then insert the function into there
			//	ie insert a map<string, map<string, function> > containing the function into the scope map
			scopeIterator->second.insert(std::make_pair(name, std::map<std::string, T >())).first->second.insert(std::make_pair(params, function));
		}
	}
	else	//  Scope doesn't even exist, add everything.
	{
		functionMap[scope][name][params] = function;
	}
}


/**
 * Generates a function to evaluate functions, used by Context's methods to avoid redundancy.
 * T	=	return type of function
 * MT	=	map's type (use decltype() for your sanity's sake)
 * @param scope The scope of the function to eval
 * @param name The name of the function to eval
 * @param parameters The parameters to feed into the function
 * @param functions The map of maps of maps that the function is stored in
 * @param typename String saying the return type, used for errors
 */
template <typename T, typename MT>
static T generateEvaluateMethod(const std::string& scope, const std::string& name, const ParameterList& parameters, const MT& functions, const std::string& typeName)
{
	auto scopeIterator = functions.find(scope);
	if (scopeIterator != functions.end())
	{
		auto nameIterator = scopeIterator->second.find(name);
		if (nameIterator != scopeIterator->second.end())
		{
			auto paramIterator = nameIterator->second.find(parameters.getSignature());
			if (paramIterator != nameIterator->second.end())
			{
				return paramIterator->second(parameters);
			}
			else
			{
				std::stringstream ss;
				ss << "Can't evaluate '"
				   << scope
				   << ":"
				   << name
				   << parameters.getFormattedSignature()
				   << " -> "
				   << typeName
				   << "' because parameter list doesn't match.";
				if (!nameIterator->second.empty())
				{
					ss << "Possible matches:\n";
					for (auto it = nameIterator->second.begin(); it != nameIterator->second.end(); ++it)
					{
						ss << "\t"
						   << scope
						   << ":"
						   << name
						   << "("
						   << it->first
						   << ")\n";
					}
				}
				else
				{
					ss << "\n\tWHAT THE FUCK HOW DID THE NAME EXIST WITH *NO* PARAMS?!";
				}
				throw RuntimeInterpreterErrorException(ss.str());
			}

		}
		else
		{
			std::stringstream ss;
			ss << "Can't evaluate '"
			   << scope
			   << ":"
			   << name
			   << parameters.getFormattedSignature()
			   << " -> "
			   << typeName
			   << "' because '"
			   << name
			   << "' was not found in scope '"
			   << scope
			   << "'";
			throw RuntimeInterpreterErrorException(ss.str());
		}
	}
	else
	{
		std::stringstream ss;
		ss << "Can't evaluate '"
		   << scope
		   << ":"
		   << name
		   << parameters.getFormattedSignature()
		   << " -> "
		   << typeName
		   << "' because scope '"
		   << scope
		   << "' doesn't even exist";
		throw RuntimeInterpreterErrorException(ss.str());
	}
}

}

#endif
