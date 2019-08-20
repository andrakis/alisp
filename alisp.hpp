// alisp.hpp
//
// Main header file for alisp interpreter.
// Include this to use alisp.
#pragma once

#include "acore.hpp"
#include "excepts.hpp"
#include "atoms.hpp"
#include "cell.hpp"
#include "environment.hpp"
#include "parser.hpp"
#include "eval.hpp"
#include "repl.hpp"
#include "stdlib.hpp"

namespace ALisp {
	// Error codes for alisp functions
	enum ErrorCode {
		NONE = 0
	};

	// Call to initialize alisp.
	// Return NONE on success, otherwise an error code.
	ErrorCode alisp_init(); // defined in core.cpp
}
