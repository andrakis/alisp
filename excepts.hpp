#pragma once

#include "acore.hpp"
#include "cell.hpp"
#include "atoms.hpp"

namespace ALisp {
	class ParserException : public Exception {
	public:
		ParserException(std::string message) : Exception(message) {}
	};

	class KeyNotFoundException : public Exception {
	public:
		KeyNotFoundException(AtomType id) : Exception(Atoms::Get(id).str()) {
		}
	};
}