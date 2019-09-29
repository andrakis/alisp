/**
 * ALisp Standard Library header
 */
#pragma once

#include "acore.hpp"
#include "cell.hpp"

namespace ALisp {
	namespace Stdlib {
		// From stdlib.cpp
		void add_stdlib(Cell env);

		// From conio.cpp
		Cell print(const ListType &args);
		// From debug.cpp
		Cell print_env(const ListType &args, EnvironmentType env);
		Cell run_tests(const ListType &args);
		Cell debug_eval(const ListType &args, EnvironmentType env);
		Cell debug_parse(const ListType &args);
		Cell debug_repl(const ListType &args, EnvironmentType env);
		// From env.cpp
		Cell env_defined(const ListType &args, EnvironmentType env);
		// From operators.cpp
		Cell plus(const ListType &args);
		Cell minus(const ListType &args);
		Cell multiply(const ListType &args);
		Cell divide(const ListType &args);
		Cell equal(const ListType &args);
		Cell nequal(const ListType &args);
		Cell lessthan(const ListType &args);
		Cell lessthanequal(const ListType &args);
		Cell morethan(const ListType &args);
		Cell morethanequal(const ListType &args);
		Cell boolnot(const ListType &args);
		Cell boolnil(const ListType &args);
		// From file.cpp
		Cell file_exists(const ListType &args);
		Cell file_path(const ListType &args);
		Cell file_read(const ListType &args);
		// From libcore.cpp
		Cell core_head(const ListType &args);
		Cell core_tail(const ListType &args);
		Cell core_size(const ListType &args);
		Cell core_size_atleast(const ListType &args);
		Cell core_index(const ListType &args);
		Cell core_list(const ListType &args);
		Cell core_none(const ListType &args);
		Cell core_atom(const ListType &args);
		Cell core_empty(const ListType &args);
		// From strings.cpp
		Cell strings_split(const ListType &args);
		Cell strings_join(const ListType &args);

		// Runtime check system
		// Each runtime_check function has a corresponding entry in message::runtime_check.
		namespace runtime_check {
			static bool has_one_argument(const ListType &args) {
				return !args.empty() && args.size() == 1;
			}
			static bool has_atleast_one_argument(const ListType &args) {
				return !args.empty();
			}
			static bool has_two_arguments(const ListType &args) {
				return args.size() == 2;
			}
		}
		namespace message {
			namespace runtime_check {
				static StringType has_one_argument = "Method requires one argument";
				static StringType has_atleast_one_argument = "Method requires at least one argument";
				static StringType has_two_arguments = "Method requires (no more than) two arguments";
			}
		}
		// Perform a check at runtime on supplied arguments.
		// Exception thrown if check returns false.
		// RUNTIME_CHECK(Args, Check) -> none() | Exception()
		// where  Args -> list()
		//   and Check -> fun((Args) -> bool()).
		#define RUNTIME_CHECK(a, check) if(!check(a)) throw Exception(message::  check)
		// Perform a check at runtime on supplied arguments.
		// Returns early with Result if check returns true.
		// RUNTIME_EARLY_RETURN(Args, Check, Result) -> none() | Result
		// where  Args -> list()
		//   and Check -> fun((Args) -> bool())
		//  and Result -> any().
		#define RUNTIME_EARLY_RETURN(a, check, result) if(check(a)) return result;

		namespace detail {
			// Argument expansion: get first argument (T first), push it,
			// then call self again with remaining arguments.
			// Execution will finally end at below invocation.
			template<class List, typename T, typename... Args>
			void push_args(List &list, T first, Args... args) {
				list.push_back(first);
				push_args(list, args...);
			}

			// End of argument expansion: one result left to push
			template<class List, typename T>
			void push_args(List &list, T value) {
				list.push_back(value);
			}
		}

		// Invoke an ALisp library function with the arguments provided.
		// The arguments are turned into the ListType required for library calls.
		// invoke_lib(Proc, Args...) -> Cell(any())
		// where Proc -> fun((Args...) -> Cell(any()))
		//   and Args... -> any(...)
		template<class Proc, typename... Args>
		Cell invoke_lib(Proc proc, Args... args) {
			ListType list;
			detail::push_args(list, args...);
			return proc(list);
		}
	}
}
