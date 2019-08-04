// alisp.hpp
//
// Main header file for alisp interpreter.
// Include this to use alisp.
#pragma once

#include "cell.hpp"
#include "environment.hpp"
#include "atoms.hpp"

namespace ALisp {
	// Error codes for alisp functions
	enum ErrorCode {
		NONE = 0
	};

	// Call to initialize alisp.
	// Return NONE on success, otherwise an error code.
	ErrorCode alisp_init(); // defined in core.cpp
}