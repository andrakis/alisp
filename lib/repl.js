/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * repl.js - Read Execute Print Loop, or interactive mode
 */

"use strict";

const readline = require('readline');
const interpreter = require('./interpreter');
const parser = require('./parser');
const version = require('./version');

function State (prompt, prompt_continuation) {
	this.exit = false;
	this.continuation = false;
	this.continued_line = "";
	this.prompt = prompt || "\x1b[1;32malisp\x1b[0m> ";
	this.prompt_continuation = prompt_continuation || "\x1b[1;32m  ...\x1b[0m> ";
	this.env = null;
	this.stacktrace = false;
}

function Command(command, modifier, note) {
	this.command = command;
	this.modifier = modifier;
	this.note = note;
}
Command.prototype.valid = function () { return this.modifier !== undefined; };
Command.prototype.toString = function() {
	var result = this.command;
	if(this.note !== undefined)
		result += ` (${this.note})`;
	return result;
};

function _exit(state) { state.exit = true; }
function _help() {
	console.error("ALisp v " + version.version);
	console.error("Available REPL commands:");
	var cmds = replCommands.map((c) => c.toString());
	console.error("\t" + cmds.join("\t"));
}
function _stacktrace(state) {
	state.stacktrace = !state.stacktrace;
	console.log("Stacktraces: " + (state.stacktrace ? "on" : "off"));
}

var replCommands = [
	new Command("exit", _exit),
	new Command("quit", _exit),
	new Command("\\q", _exit),
	new Command("help", _help),
	new Command("\\h", _help),
	new Command("\\st", _stacktrace, "Toggle stack traces"),
	new Command()
];

function getCommand(cmd) {
	for (var i = 0; i < replCommands.length; ++i)
		if (replCommands[i].command === cmd)
			return replCommands[i];
	return null;
}

function invokeCommand(cmd, state) {
	var command = getCommand(cmd);
	if (command !== null)
		command.modifier(state);
}

var state = new State();
function read() {
	// Update prompt state
	if (state.continuation)
		rl.setPrompt(state.prompt_continuation);
	else
		rl.setPrompt(state.prompt);
	rl.prompt();
}
function repl_exec_error (e) {
	console.error("Error: ", e.message);
	if (state.stacktrace)
		console.error(e.stack);
	loop();
}
function repl_exec_success (result) {
	print(result);
}
function execute(line) {
	line = line.trim();
	if (line === '')
		return loop();
	if (state.continuation) {
		line = state.continued_line + line;
		state.continuation = false;
	}
	var cmd = getCommand(line);
	if (cmd !== null) {
		cmd.modifier(state);
		return loop();
	}

	// Attempt to parse
	var parsed;
	try {
		parsed = parser.read(line);
	} catch (e) {
		state.continuation = true;
		state.continued_line = line;
		loop();
	}

	// Execute
	interpreter.execute(parsed, state.env)
		.on('error', repl_exec_error)
		.on('success', repl_exec_success)
		.start();
}
function print(result) {
	console.log(result.toString());
	loop();
}
function loop() {
	if(state.exit) {
		rl.close();
	} else {
		read();
	}
}
var init_done = false;
var rl;
function init() {
	rl = readline.createInterface({
		input: process.stdin,
		output: process.stdout,
		prompt: 'ALisp> '
	});
	rl.on('line', (line) => {
		execute(line);
	});
	rl.on('close', () => {
		state.exit = true;
	});
}

function REPL(envCell) {
	if(!init_done) {
		init_done = true;
		init();
	}
	state.env = envCell;
	invokeCommand("help", state);
	read();
}

exports.REPL = REPL;
