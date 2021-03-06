// acore.hpp - ALisp core header
//
// Included by most files for basic type information.
// Should not be included by itself in other programs.
#pragma once

#include <exception>
#include <memory>
#include <string>
#include <vector>

// Macros
// Because VC++ complains about ignoring throw(...) sections
#if _MSC_VER
#define THROW(x) throw(...)
#else
#define THROW(x) throw(x)
#endif

// Standard exceptions
class Exception : public std::exception {
public:
#if _MSC_VER
	Exception(std::string message) : std::exception(message.c_str()) { }
#else
	std::string msg;
	Exception(std::string message) : msg(message) { }
	#ifndef _GLIBCXX_TXN_SAFE_DYN 
	  #define _GLIBCXX_TXN_SAFE_DYN 
	#endif
	#ifndef _GLIBCXX_USE_NOEXCEPT 
	  #define _GLIBCXX_USE_NOEXCEPT 
	#endif
	const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT { return msg.c_str(); }
#endif
};

#define EXCEPT	THROW(Exception)

#ifdef _MSC_VER
  #define NOMINMAX
  #include <Windows.h>
  #define debugger_attached() IsDebuggerPresent()
#else
  #define debugger_attached() false
#endif

namespace ALisp {
	struct Cell; // From cell.hpp
	class Environment; // From environment.hpp

	// Basic types
	typedef size_t AtomType;
	typedef std::string StringType;
	typedef long IntegerType;
	typedef double FloatType;
	typedef std::vector<Cell> ListType;
	typedef std::shared_ptr<Environment> EnvironmentType;
	typedef std::weak_ptr<Environment> EnvironmentReference;

	// Callable function types
	typedef Cell(*ProcType)(const ListType &args);
	typedef Cell(*ProcEnvType)(const ListType &args, EnvironmentType env);

	// Always-present cells (see core.cpp)
	extern Cell Nil, True, False;
	extern AtomType _if, _quote, _define, _set, _lambda, _macro, _begin;
}

