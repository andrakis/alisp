#include <iostream>

#include "alisp.hpp"

using namespace ALisp;
using namespace Atoms;
using namespace REPL;

void test_eval() {
	StringType code = "(begin (define fac (lambda (n) (fac/2 n 1))) (define fac/2 (lambda (n a) (if (= 1 n) a (fac/2 (- n 1) (* n a))))) (define x 10) (print \"Fac\" x(fac x)))";
	Cell code_cell = Parser::read(code);
	Environment *env_ptr = new Environment();
	EnvironmentType env_t(env_ptr);

	env_t->create(Declare("print"), ProcCell(Stdlib::print));
	env_t->create(Declare("+"), ProcCell(Stdlib::plus));
	env_t->create(Declare("-"), ProcCell(Stdlib::minus));
	env_t->create(Declare("*"), ProcCell(Stdlib::multiply));
	env_t->create(Declare("="), ProcCell(Stdlib::equal));

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
	Stdlib::print(args);

	ListCell args2;
	args2.push(three);
	args2.push(two);
	args2.push(one);
	std::cout << " At 0: " << args2.index(0) << std::endl;
	std::cout << " At 1: " << args2.index(1) << std::endl;
	std::cout << " At 2: " << args2.index(2) << std::endl;
	std::cout << " At 3: " << args2.index(3) << std::endl;
	std::cout << "Args: " << args2 << std::endl;
	Stdlib::print(ListType(args2.cbegin(), args2.cend()));

	ProcCell print(Stdlib::print);
	print.proc()(ListType(args2.cbegin(), args2.cend()));

	test_eval();
}

