/**
 * Standard Library - Operators
 *
 * Defines the standard operators: + - * / == != < > and variants.
 *
 * Mathematical operators: + - * / << >>
 *   - Mathematical operators can take any number of arguments
 *
 * Conditional operators: == != < >
 *   - Operate on two values only
 */

#include "../alisp.hpp"
#include "../stdlib.hpp"

namespace ALisp {
	namespace Stdlib {
		Cell plus(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_atleast_one_argument);
			RUNTIME_EARLY_RETURN(args, runtime_check::has_one_argument, args[0]);
			auto it = args.cbegin();
			Cell value(*it++);
			for (; it != args.cend(); ++it)
				value += *it;
			return value;
		}

		Cell minus(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_atleast_one_argument);
			RUNTIME_EARLY_RETURN(args, runtime_check::has_one_argument, args[0]);
			auto it = args.cbegin();
			Cell value(*it++);
			for (; it != args.cend(); ++it)
				value -= *it;
			return value;
		}

		Cell multiply(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_atleast_one_argument);
			RUNTIME_EARLY_RETURN(args, runtime_check::has_one_argument, args[0]);
			auto it = args.cbegin();
			Cell value(*it++);
			for (; it != args.cend(); ++it)
				value *= *it;
			return value;
		}

		Cell divide(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_atleast_one_argument);
			RUNTIME_EARLY_RETURN(args, runtime_check::has_one_argument, args[0]);
			auto it = args.cbegin();
			Cell value(*it++);
			for (; it != args.cend(); ++it)
				value /= *it;
			return value;
		}

		// Comparison operators

		Cell equal(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_two_arguments);
			return args[0] == args[1] ? True : False;
		}

		Cell nequal(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_two_arguments);
			return args[0] != args[1] ? True : False;
		}

		Cell lessthan(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_two_arguments);
			return args[0] < args[1] ? True : False;
		}

		Cell lessthanequal(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_two_arguments);
			return args[0] <= args[1] ? True : False;
		}

		Cell morethan(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_two_arguments);
			return args[0] > args[1] ? True : False;
		}

		Cell morethanequal(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_two_arguments);
			return args[0] >= args[1] ? True : False;
		}
	}
}