#pragma once

#include "acore.hpp"

namespace ALisp {
	namespace REPL {
		struct State {
			bool exit = false;
			bool continuation = false;
			StringType prompt = "\x1b[1;32malisp\x1b[0m> ";
			StringType prompt_continuation = "\x1b[1;32m  ...\x1b[0m> ";
			StringType continued_line;

			State() {}
			State(StringType _prompt) : prompt(_prompt) {}
		};
		typedef void(*Modifier)(State&);

		struct Command {
			StringType command;
			Modifier modifier;
			Command() : command(""), modifier(nullptr) {}
			Command(StringType cmd, Modifier mod) : command(cmd), modifier(mod) {}
			bool valid() const { return modifier != nullptr; }
		};

		extern Command replCommands[];
		Command *getCommand(StringType cmd);
		void invokeCommand(StringType cmd, State &);
		void REPL(Cell env);
		void shutdown_repl();
	}
}
