#pragma once

#include "acore.hpp"
#include "cell.hpp"
#include "environment.hpp"
#include "atoms.hpp"

namespace ALisp {
	namespace Eval {
		namespace Simple {
			Cell eval(const Cell &x, const Cell &env);
		}
	}
}