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

namespace ALisp {
	namespace Stdlib {
		Cell plus(const ListType &args) {
			auto it = args.cbegin();
			Cell value(*it++);
			for (; it != args.cend(); ++it)
				value += *it;
			return value;
		}

		Cell minus(const ListType &args) {
			auto it = args.cbegin();
			Cell value(*it++);
			for (; it != args.cend(); ++it)
				value -= *it;
			return value;
		}

		Cell multiply(const ListType &args) {
			auto it = args.cbegin();
			Cell value(*it++);
			for (; it != args.cend(); ++it)
				value *= *it;
			return value;
		}

		Cell divide(const ListType &args) {
			auto it = args.cbegin();
			Cell value(*it++);
			for (; it != args.cend(); ++it)
				value /= *it;
			return value;
		}

		// Comparison operators

		Cell equal(const ListType &args) {
			return args[0] == args[1] ? True : False;
		}

		Cell nequal(const ListType &args) {
			return args[0] != args[1] ? True : False;
		}

		Cell lessthan(const ListType &args) {
			return args[0] < args[1] ? True : False;
		}

		Cell lessthanequal(const ListType &args) {
			return args[0] <= args[1] ? True : False;
		}

		Cell morethan(const ListType &args) {
			return args[0] > args[1] ? True : False;
		}

		Cell morethanequal(const ListType &args) {
			return args[0] >= args[1] ? True : False;
		}
	}
}