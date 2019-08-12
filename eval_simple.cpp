#include "eval_simple.hpp"

namespace ALisp {
	namespace Eval {
		namespace Simple {
			Cell eval(const Cell &_x, const Cell &_env) {
				Cell x = _x;
				Cell env = _env;
			recurse:
				switch (x.type()) {
					case CellType::Atom:
						return env.env()->get(x.atomId());
					case CellType::String:
					case CellType::Integer:
					case CellType::Float:
						return x;
				}
				if (x.empty()) return Nil;

				Cell sym = x.head();
				if (sym.type() == CellType::Atom) {
					AtomType id = sym.atomId();
					if (id == _quote) {			// (quote exp)
						return x.index(1);
					} else if (id == _if) {
						Cell test = x.index(1);
						Cell conseq = x.index(2);
						Cell alt = x.index(3); // default: Nil
						Cell result = eval(test, env);
						x = (result == False) ? alt : conseq;
						goto recurse;
					} else if (id == _define) {		// (define var exp) - create new variable
						AtomType newid = x.index(1).atomId();
						x = eval(x.index(2), env);
						env.env()->create(newid, x);
						return x;
					} else if (id == _set) {		// (set! var exp) - update existing variable
						AtomType newid = x.index(1).atomId();
						x = eval(x.index(2), env);
						env.env()->set(newid, x);
						return x;
					} else if (id == _lambda) {		// (lambda (var*) exp)
						LambdaCell lambda;
						lambda.lambda_ptr(env);
						lambda.lambda_args(x.index(1));
						lambda.lambda_body(x.index(2));
						return lambda;
					} else if (id == _macro) {		// (macro (var*) exp)
						MacroCell macro;
						macro.lambda_ptr(env);
						macro.lambda_args(x.index(1));
						macro.lambda_body(x.index(2));
						return macro;
					} else if (id == _begin) {		// (begin exp*)
						auto it = x.cbegin() + 1;
						for (; it != x.cend() - 1; ++it) {
							eval(*it, env);
						}
						x = *it;
						goto recurse;
					}
				}

				// (proc exp*)
				Cell proc = eval(x.index(0), env);
				ListType exps;
				if (proc.type() == CellType::Macro)
					exps = ListType(x.tail().cbegin(), x.tail().cend());
				else {
					// (map (tail x) (lambda (y) (eval y env)))
					for (auto it = x.cbegin() + 1; it != x.cend(); ++it)
						exps.push_back(eval(*it, env));
				}
				switch (proc.type()) {
					case CellType::Lambda: {
						Environment *env_ptr = new Environment(proc.lambda_args(), exps, proc.lambda_ptr().env());
						EnvironmentType env_shared(env_ptr);
						env = EnvironmentCell(env_shared); // swap environments
						x = proc.lambda_body();
						goto recurse;
					}
					case CellType::Proc:
						return proc.proc()(exps);
					case CellType::ProcEnv:
						return proc.proc_env()(exps, env.env());
					default:
						throw Exception("Not an executable cell");
				}
			}
		}
	}
}