#include "alisp.hpp"

using namespace ALisp::Atoms;

namespace ALisp {
	namespace Stdlib {
		void add_stdlib(Cell env) {
			EnvironmentType ep(env.env());
			ep->create(Declare("print"), ProcCell(print));
			ep->create(Declare("+"), ProcCell(plus)); ep->create(Declare("-"), ProcCell(minus));
			ep->create(Declare("*"), ProcCell(multiply)); ep->create(Declare("/"), ProcCell(divide));
			ep->create(Declare("="), ProcCell(equal)); ep->create(Declare("=="), ProcCell(equal));
			ep->create(Declare("!="), ProcCell(nequal));
			ep->create(Declare("<"), ProcCell(lessthan)); ep->create(Declare("<="), ProcCell(lessthanequal));
			ep->create(Declare(">"), ProcCell(morethan)); ep->create(Declare(">="), ProcCell(morethanequal));
			ep->create(Declare("penv"), ProcEnvCell(print_env));
		}
	}
}
