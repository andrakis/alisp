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
#include <string>
#include <string.h>
#include <vector>

#include "alisp.hpp"
#include "stdlib.hpp"
#include "timekeeping.hpp"
#include "version.hpp"

using namespace ALisp;
using namespace Stdlib;

struct {
	bool interactive = false; // -i REPL mode
	bool timing = false;      // -t Display timing information
	bool tests = false;       // -T Run tests
	bool debug = false;       // -d Debug mode
	bool help = false;        // --help, -h
	StringType file = "";
	ListType args;

	bool did_anything = false;
	bool has_file() const {
		return file != "";
	}

	void readFrom(int argc, char **argv) {
		int i = 1; ++argv;
		while (i < argc) {
			if (!strcmp(*argv, "--help") || !strcmp(*argv, "-h"))
				help = true;
			else if (!strcmp(*argv, "-i"))
				interactive = true;
			else if (!strcmp(*argv, "-t"))
				timing = true;
			else if (!strcmp(*argv, "-T"))
				tests = true;
			else if (!strcmp(*argv, "-d"))
				debug = true;
			else if (!has_file())
				file = StringType(argv[0]);
			else {
				args.push_back(StringCell(StringType(argv[0])));
			}
			++i; ++argv;
		}
	}
} MainState;

// From tests.cpp
void test_eval();
void test();

void show_help() {
	std::cerr << "ALisp v " << alisp_version_str << std::endl;
	std::cerr << std::endl;
	std::cerr << "Usage:" << std::endl;
	std::cerr << "alisp [-i] [-t] [-T] [-D] [file] [args]" << std::endl;
	std::cerr << "Where:" << std::endl;
	std::cerr << "\t-i             Interactive mode (REPL)" << std::endl;
	std::cerr << "\t-t             Display timing information" << std::endl;
	std::cerr << "\t-T             Run tests" << std::endl;
	std::cerr << "\t-d             Debug mode" << std::endl;
	std::cerr << "\tfile           File to parse and execute" << std::endl;
	std::cerr << "\targs           Arguments to pass" << std::endl;
}

int main(int argc, char **argv) {
	MainState.readFrom(argc, argv);

	ErrorCode error_code;
	switch (error_code = alisp_init()) {
		case ErrorCode::NONE:
			// All good
			break;
		default:
			std::cerr << "alisp_init() returned " << error_code << std::endl;
			return 1;
	}

	auto after_startup = Atoms::Count();
	if (MainState.debug)
		std::cerr << "(dbg) " << after_startup << " atoms created at startup." << std::endl;

	EnvironmentCell ec(std::make_shared<Environment>());
	Stdlib::add_stdlib(ec);
	auto after_stdlib = Atoms::Count();
	if (MainState.debug)
		std::cerr << "(dbg) " << (after_stdlib - after_startup) << " atoms created at add_stdlib." << std::endl;

	if (MainState.help) {
		MainState.did_anything = true;
		show_help();
	} else if (MainState.tests) {
		MainState.did_anything = true;
		test();
	} else if (MainState.interactive) {
		MainState.did_anything = true;
		REPL::REPL(ec);
	} else if (MainState.has_file()) {
		MainState.did_anything = true;
		try {
			Cell full_path = invoke_lib(file_path, StringCell(MainState.file));
			if (invoke_lib(file_exists, full_path) == False) {
				std::cerr << "Failed to open " << full_path << std::endl;
				return 2;
			}
			Cell read = invoke_lib(file_read, full_path);
			auto parse_call = Timekeeping::timeCallbackResult<Cell>([&read] () {
				return Parser::read(read.str());
			});
			Cell parsed = std::get<1>(parse_call);
			if (MainState.timing)
				std::cerr << "(tmg) Parsing took " << std::get<0>(parse_call) << Timekeeping::time_type() << std::endl;
			auto after_parsing = Atoms::Count();
			if (MainState.debug)
				std::cerr << "(dbg) " << (after_parsing - after_stdlib) << " atoms created during parsing." << std::endl;
			// Execute the code
			auto eval_call = Timekeeping::timeCallbackResult<Cell>([&parsed, &ec] () {
				return Eval::Simple::eval(parsed, ec);
			});
			if (MainState.timing)
				std::cerr << "(tmg) Execution took " << std::get<0>(eval_call) << Timekeeping::time_type() << std::endl;
			if (MainState.debug) {
				std::cerr << "(dbg) Final result: " << std::get<1>(eval_call) << std::endl;
				std::cerr << "(dbg) Final atom count: " << Atoms::Count() << std::endl;
			}
		} catch (Exception &e) {
			std::cerr << "Exception: " << e.what() << std::endl;
			return 3;
		} catch (...) {
			std::cerr << "Unknown error occurred" << std::endl;
			return 3;
		}
	}

	auto clear_time = Timekeeping::timeCall([&ec] () {
		ec.env()->clear();
	});
	if (MainState.timing)
		std::cerr << "(tmg) Cleanup took " << clear_time << Timekeeping::time_type() << std::endl;

	if (MainState.did_anything == false) {
		show_help();
		return 1;
	}

	if (debugger_attached()) {
		std::cerr << "Press enter to exit";
		while (std::cin.get() != '\n') /* Nothing */;
	}
	return 0;
}
