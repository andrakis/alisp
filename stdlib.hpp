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
	}
}
