#include "alisp.hpp"

namespace ALisp {
	// Standard library atoms
	Cell Nil, True, False;

	// Eval atoms
	AtomType _if, _quote, _define, _set, _lambda, _macro, _begin;

	ErrorCode alisp_init() {
		Nil = Atoms::Declare("nil");
		True = Atoms::Declare("true");
		False = Atoms::Declare("false");
		_quote = Atoms::Declare("quote").atomId();
		_if = Atoms::Declare("if").atomId();
		_define = Atoms::Declare("define").atomId();
		_set = Atoms::Declare("set!").atomId();
		_lambda = Atoms::Declare("lambda").atomId();
		_macro = Atoms::Declare("macro").atomId();
		_begin = Atoms::Declare("begin").atomId();

		return ErrorCode::NONE;
	}
}
