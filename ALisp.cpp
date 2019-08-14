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

Cell test_plus(const ListType &args) {
	auto it = args.cbegin();
	Cell value(*it++);
	for (; it != args.cend(); ++it)
		value += *it;
	return value;
}

Cell test_minus(const ListType &args) {
	auto it = args.cbegin();
	Cell value(*it++);
	for (; it != args.cend(); ++it)
		value -= *it;
	return value;
}

Cell test_multiply(const ListType &args) {
	auto it = args.cbegin();
	Cell value(*it++);
	for (; it != args.cend(); ++it)
		value *= *it;
	return value;
}

Cell test_equal(const ListType &args) {
	return args[0] == args[1] ? True : False;
}

using namespace Atoms;

void test_eval() {
	StringType code = "(begin (define fac (lambda (n) (fac/2 n 1))) (define fac/2 (lambda (n a) (if (= 1 n) a (fac/2 (- n 1) (* n a))))) (define x 10) (print \"Fac\" x(fac x)))";
	Cell code_cell = Parser::read(code);
	Environment *env_ptr = new Environment();
	EnvironmentType env_t(env_ptr);

	env_t->create(Declare("print"), ProcCell(test_print));
	env_t->create(Declare("+"), ProcCell(test_plus));
	env_t->create(Declare("-"), ProcCell(test_minus));
	env_t->create(Declare("*"), ProcCell(test_multiply));
	env_t->create(Declare("="), ProcCell(test_equal));

	EnvironmentCell env_cell(env_t);

	Cell result = Eval::Simple::eval(code_cell, env_cell);
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
	std::cout << " At 0: " << args2.index(0) << std::endl;
	std::cout << " At 1: " << args2.index(1) << std::endl;
	std::cout << " At 2: " << args2.index(2) << std::endl;
	std::cout << " At 3: " << args2.index(3) << std::endl;
	std::cout << "Args: " << args2 << std::endl;
	test_print(ListType(args2.cbegin(), args2.cend()));

	ProcCell print(test_print);
	print.proc()(ListType(args2.cbegin(), args2.cend()));

	test_eval();
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
