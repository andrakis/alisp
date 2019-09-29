#include <iostream>
#include <string.h>

#include <linenoise.h>

#include "alisp.hpp"
#include "repl.hpp"
#include "version.hpp"

namespace ALisp {
	namespace REPL {
		void _exit(State &state) { state.exit = true; }
		void _help(State &state) {
			std::cout << "ALisp v " alisp_version_str << std::endl;
			std::cout << "Tab autocompletion available." << std::endl;
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
		const char *builtins[] = {
			"quote", "if", "define", "set!", "lambda", "macro", "begin",
			NULL
		};

		EnvironmentReference last_env;
		bool linenoise_initialized = false;
		const char *linenoise_history = "./history";
		void completion_hook(char const *prefix, linenoiseCompletions *lc) {
			size_t len = strlen(prefix);
			
			// Add builtins
			for(size_t i = 0; builtins[i] != NULL; ++i) {
				if (!strncmp(builtins[i], prefix, len))
					linenoiseAddCompletion(lc, builtins[i]);
			}

			// Add REPL commands
			for (auto it = &replCommands[0]; it->valid(); ++it) {
				if (!strncmp(it->command.c_str(), prefix, len))
					linenoiseAddCompletion(lc, it->command.c_str());
			}

			// Add environment atoms.
			// These require a reference to the current environment
			if (auto env = last_env.lock()) {
				auto e = env;
				for (;;) {
					for (auto it = e->cbegin(); it != e->cend(); ++it) {
						Cell atom = Atoms::Get(it->first);
						if (!strncmp(atom.str().c_str(), prefix, len)) {
							// Got a partial match
							linenoiseAddCompletion(lc, atom.str().c_str());
						}
					}
					if (!e->has_parent())
						break;
					e = e->get_parent();
				}
			}
		}

		void check_linenoise() {
			if (linenoise_initialized) return;
			
			linenoiseInstallWindowChangeHandler();
			linenoiseHistoryLoad(linenoise_history);
			linenoiseSetCompletionCallback(completion_hook);

			linenoise_initialized = true;
		}
		void shutdown_repl() {
			if (!linenoise_initialized) return;

			linenoiseHistorySave(linenoise_history);
			linenoiseHistoryFree();
		}

		void REPL(Cell env) {
			State state;
			Eval::EvalType evaluator = Eval::Simple::eval;
			ListType history;
			last_env = env.env();

			check_linenoise();

			invokeCommand("help", state);
			while (state.exit == false) {
				const char *prompt = state.prompt.c_str();
				if (state.continuation)
					prompt = state.prompt_continuation.c_str();
				char *line_result = linenoise(prompt);
				if (line_result == NULL)
					break;

				StringType line(line_result);
				// No longer need string allocated
				free(line_result);

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

				// Parsed successfully, save and execute
				linenoiseHistoryAdd(line.c_str());

				// Execute
				try {
					Cell result = evaluator(read, env);
					history.push_back(result);
					std::cout << result.repr() << std::endl;
				} catch (KeyNotFoundException &e) {
					std::cerr << "Key not found: " << e.what() << std::endl;
				} catch (Exception &e) {
					std::cerr << "Exception: " << e.what() << std::endl;
				}
			}

			last_env.reset();
		}
	}
}
