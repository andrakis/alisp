#include "alisp.hpp"

using namespace ALisp;

ErrorCode ALisp::alisp_init() {
	Atoms::Declare("nil", Nil.atomId());
	Atoms::Declare("true", True.atomId());
	Atoms::Declare("false", False.atomId());
	Atoms::detail::atom_counter = 3;

	return ErrorCode::NONE;
}