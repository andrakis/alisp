#include <iostream>

#include "alisp.hpp"
#include "version.hpp"

namespace ALisp {
	namespace REPL {
		void _exit(State &state) { state.exit = true; }
		void _help(State &state) {
			std::cout << "ALisp v " alisp_version_str << std::endl;
			std::cout << "Available REPL commands: " << std::endl;
			for (auto it = &replCommands[0]; it->valid(); ++it)
				std::cout << "\t" << it->command;
			std::cout << std::endl;
		}

		Command *getCommand(StringType cmd) {
			for (auto it = &replCommands[0]; it->valid(); ++it)
				if (it->command == cmd)
					return it;
			return nullptr;
		}

		void invokeCommand(StringType cmd, State &state) {
			Command *r = getCommand(cmd);
			if (r != nullptr)
				r->modifier(state);
		}

		Command replCommands[] ={
			Command("exit", _exit),
			Command("quit", _exit),
			Command("help", _help),
			Command("\\h", _help),
			Command("\\q", _exit),
			Command()
		};

		void REPL(Cell env) {
			State state;
			Eval::EvalType evaluator = Eval::Simple::eval;
			ListType history;

			invokeCommand("help", state);
			while (state.exit == false) {
				if (state.continuation)
					std::cout << " ... ";
				else
					std::cout << state.prompt;
				StringType line; std::getline(std::cin, line);
				if (state.continuation) {
					line = state.continued_line + line;
					state.continuation = false;
				}
				// Check if line is a repl command
				Command *cmd = getCommand(line);
				if (cmd != nullptr) {
					cmd->modifier(state);
					continue;
				}

				// Attempt to parse
				Cell read;
				try {
					read = Parser::read(line);
				} catch (ParserException const &pe) {
					state.continuation = true;
					state.continued_line = line;
					continue;
				}

				// Parsed successfully, execute
				try {
					Cell result = evaluator(read, env);
					history.push_back(result);
					std::cout << result << std::endl;
				} catch (KeyNotFoundException &e) {
					std::cerr << "Key not found: " << e.what() << std::endl;
				} catch (Exception &e) {
					std::cerr << "Exception: " << e.what() << std::endl;
				}
			}
		}
	}
}
