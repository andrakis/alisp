/**
 * ALisp - A Lisp implementation in C++
 *
 * Supported types:
 *  - Atom (symbol stored as atom id)
 *  - String
 *  - Integer (whole number; long type)
 *  - Float (double type)
 *  - List (vector)
 *  - Environment (pointer to environment)
 *  - Lambda (List+Environment)
 *  - Macro (List+Environment)
 *  - Proc (callable function)
 *  - ProcEnv (callable function that also takes current environment)
 */

#include <iostream>

#include "alisp.hpp"

using namespace ALisp;

void test() {
	IntegerCell one(1), two(2), three(3);
	FloatCell approx_one(1.001), approx_two(2.2), approx_three(3.33333);

	std::cout << "One: " << one << std::endl;
	std::cout << "Two: " << two << std::endl;
	std::cout << "Three: " << three << std::endl;

	std::cout << "Approximately one: " << approx_one << std::endl;
	std::cout << "Approximately two: " << approx_two << std::endl;
	std::cout << "Approximately three: " << approx_three << std::endl;

	Cell added = one.add(two);
	std::cout << "One plus two: " << added << std::endl;
}

int main(int argc, char **argv) {
	alisp_init();
	test();

	if (debugger_attached()) {
		std::cerr << "Press enter to exit";
		while (std::cin.get() != '\n') /* Nothing */;
	}
	return 0;
}
