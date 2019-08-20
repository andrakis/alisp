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

		// Runtime checks
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
		#define RUNTIME_CHECK(a, check) if(!check(a)) throw Exception(message::  check)
		#define RUNTIME_EARLY_RETURN(a, check, result) if(check(a)) return result;

		template<class List, typename T>
		void push_args(List &list, T value) {
			list.push_back(value);
		}
		template<class List, typename T, typename... Args>
		void push_args(List &list, T first, Args... args) {
			list.push_back(first);
			push_args(list, args...);
		}
		template<class Proc, typename... Args>
		Cell invoke_lib(Proc proc, Args... args) {
			ListType list;
			push_args(list, args...);
			return proc(list);
		}
	}
}
