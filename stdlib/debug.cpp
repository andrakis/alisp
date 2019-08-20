/** 
 * Stdlib - Debug functions
 */

#include <iostream>

#include "../alisp.hpp"
#include "../stdlib.hpp"
#include "../parser.hpp"
#include "../eval.hpp"
#include "../repl.hpp"

void test(); // tests.cpp

namespace ALisp {
	namespace Stdlib {
		size_t depthCount(size_t n, EnvironmentType env) {
			if (env->has_parent())
				return depthCount(n + 1, env->get_parent());
			return n;
		}
		size_t depthCount(EnvironmentType env) {
			return depthCount(1, env);
		}
		Cell print_env(const ListType &args, EnvironmentType env) {
			// Get depth count
			size_t depth = depthCount(env);
			auto e = env;
			for (;;) {
				for (auto it = e->cbegin(); it != e->cend(); ++it) {
					std::cout << std::string(depth, '-') << "> ";
					std::cout << Atoms::Get(it->first) << " = " << it->second << std::endl;
				}
				if (!e->has_parent())
					break;
				e = e->get_parent();
				--depth;
			}
			return Nil;
		}

		Cell run_tests(const ListType &args) {
			test();
			return Nil;
		}

		Cell debug_repl(const ListType &args, EnvironmentType env) {
			REPL::REPL(EnvironmentCell(env));
			return Nil;
		}

		// Debug parsing function

		// (debug:parse Content) -> cells()
		Cell debug_parse(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_one_argument);
			return Parser::read(args[0].str());
		}
		// (debug:eval Content) -> cell()
		Cell debug_eval(const ListType &args, EnvironmentType env) {
			RUNTIME_CHECK(args, runtime_check::has_atleast_one_argument);
			Cell env_used;

			if (runtime_check::has_two_arguments(args))
				env_used = args[1];
			else
				env_used = EnvironmentCell(env);
			return Eval::Simple::eval(args[0], env_used);
		}
	}
}