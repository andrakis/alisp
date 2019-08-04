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
	const char *what() const  _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT { return msg.c_str(); }
#endif
};

#define EXCEPT	THROW(Exception)

#ifdef _MSC_VER
  #include <Windows.h>
  #define debugger_attached() IsDebuggerPresent()
  // Windows.h defines these, but we don't want their definitions
  #undef min
  #undef max
#else
  #define debugger_attached() false
#endif

namespace ALisp {
	class Cell; // From cell.hpp
	class Environment; // From environment.hpp

	// Basic types
	typedef size_t AtomType;
	typedef std::string StringType;
	typedef long IntegerType;
	typedef double FloatType;
	typedef std::vector<Cell> ListType;
	typedef std::shared_ptr<Environment> EnvironmentType;

	// Callable function types
	typedef Cell(*ProcType)(ListType &&args);
	typedef Cell(*ProcEnvType)(ListType &&args, EnvironmentType &&env);
}

