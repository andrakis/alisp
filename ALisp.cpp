/**
 * ALisp - A Lisp implementation in C++
 *
 * Supported types:
 *  - Atom (symbol stored as atom id)
 *  - String
 *  - Integer (whole number; long type)
 *  - Float (double type)
 *  - List (vector)
 *  - Lambda (List in form (<EnvPtr> <Args> <Body>))
 *  - Macro (List in form (<EnvPtr> <Args> <Body>))
 *  - Proc (callable function)
 *  - ProcEnv (callable function that also takes current environment)
 *  - Environment (pointer to environment)
 */

#include <iostream>

#include "alisp.hpp"

using namespace ALisp;

Cell test_print(const ListType &args) {
	for (auto it = args.cbegin(); it != args.cend(); ++it) {
		if (it != args.cbegin())
			std::cout << " ";
		std::cout << it->str();
	}
	std::cout << std::endl;
	return Nil;
}

void test() {
	Cell one(IntCell(1)), two(IntCell(2));
	Cell three(one);
	three += two;

	std::cout << "One: " << one << std::endl;
	std::cout << "Two: " << two << std::endl;
	std::cout << "Three: " << three << std::endl;

	std::cout << "Nil: " << Nil << std::endl;
	std::cout << "True: " << True << std::endl;
	std::cout << "False: " << False << std::endl;

	ListType args;
	args.push_back(one);
	args.push_back(two);
	args.push_back(three);
	test_print(args);

	ListCell args2;
	args2.push(three);
	args2.push(two);
	args2.push(one);
	std::cout << "Args: " << args2 << std::endl;
	test_print(ListType(args2.cbegin(), args2.cend()));

	ProcCell print(test_print);
	print.proc()(ListType(args2.cbegin(), args2.cend()));
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
