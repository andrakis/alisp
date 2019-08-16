#pragma once

#include "eval_simple.hpp"

namespace ALisp {
	namespace Eval {
		typedef Cell(*EvalType)(const Cell&, const Cell&);
	}
}
