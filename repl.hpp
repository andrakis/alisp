#pragma once

#include "acore.hpp"

namespace ALisp {
	namespace REPL {
		struct State {
			bool exit = false;
			bool continuation = false;
			StringType prompt;
			StringType continued_line;

			State(StringType prompt = "> ") : prompt(prompt) {}
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
	}
}
