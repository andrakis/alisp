#include "alisp.hpp"

using namespace ALisp::Atoms;

namespace ALisp {
	namespace Stdlib {
		void add_stdlib(Cell env) {
			EnvironmentType ep(env.env());
			// Global symbols
			ep->create(Declare("true"), True);
			ep->create(Declare("false"), False);
			ep->create(Declare("nil"), Nil);

			// conio.cpp
			ep->create(Declare("print"), ProcCell(print));
			// operators.cpp
			ep->create(Declare("+"), ProcCell(plus)); ep->create(Declare("-"), ProcCell(minus));
			ep->create(Declare("*"), ProcCell(multiply)); ep->create(Declare("/"), ProcCell(divide));
			ep->create(Declare("="), ProcCell(equal)); ep->create(Declare("=="), ProcCell(equal));
			ep->create(Declare("!="), ProcCell(nequal));
			ep->create(Declare("<"), ProcCell(lessthan)); ep->create(Declare("<="), ProcCell(lessthanequal));
			ep->create(Declare(">"), ProcCell(morethan)); ep->create(Declare(">="), ProcCell(morethanequal));
			// debug.cpp
			ep->create(Declare("penv"), ProcEnvCell(print_env));
			ep->create(Declare("run_tests"), ProcCell(run_tests));
			ep->create(Declare("debug:parse"), ProcCell(debug_parse));
			ep->create(Declare("debug:eval"), ProcEnvCell(debug_eval));
			ep->create(Declare("debug:repl"), ProcEnvCell(debug_repl));
			// file.cpp
			ep->create(Declare("file:exists"), ProcCell(file_exists));
			ep->create(Declare("file:path"), ProcCell(file_path));
			ep->create(Declare("file:read"), ProcCell(file_read));
			// libcore.cpp
			ep->create(Declare("head"), ProcCell(core_head));
			ep->create(Declare("tail"), ProcCell(core_tail));
			ep->create(Declare("size"), ProcCell(core_size));
			ep->create(Declare("size_atleast"), ProcCell(core_size_atleast));
			ep->create(Declare("index"), ProcCell(core_index));
			ep->create(Declare("list"), ProcCell(core_list));
		}
	}
}
