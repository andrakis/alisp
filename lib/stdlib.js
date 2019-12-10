/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * stdlib - contains the standard library
 */

"use strict";

var atoms = require('./atoms');
var types = require('./types');
var ALVar = types.ALVar, type = types.type;
var core = require('./stdlib/core');
var conio = require('./stdlib/conio');
var debug = require('./stdlib/debug');
var environment = require('./stdlib/environment');
var file = require('./stdlib/file');
var operators = require('./stdlib/operators');
var strings = require('./stdlib/strings');

// Export sub items
exports.libs = {
	core: core,
	conio: conio,
	debug: debug,
	environment: environment,
	file: file,
	operators: operators,
	strings: strings
};

// Function for invoking library functions
//exports.invoke_lib = function(

function add_stdlib(env) {
	env.define("true", atoms.symbols.True);
	env.define("false", atoms.symbols.False);
	env.define("nil", atoms.symbols.Nil);
	env.define("print", new ALVar(conio.proc_print, type.PROC));
	env.define("+", new ALVar(operators.proc_plus, type.PROC));
	env.define("-", new ALVar(operators.proc_minus, type.PROC));
	env.define("*", new ALVar(operators.proc_multiply, type.PROC));
	env.define("/", new ALVar(operators.proc_divide, type.PROC));
	env.define("=", new ALVar(operators.proc_equal, type.PROC));
	env.define("<", new ALVar(operators.proc_lessthan, type.PROC));
	env.define("<=", new ALVar(operators.proc_lessthanequal, type.PROC));
	env.define(">", new ALVar(operators.proc_morethan, type.PROC));
	env.define(">=", new ALVar(operators.proc_morethanequal, type.PROC));
	env.define("not", new ALVar(operators.proc_not, type.PROC));
	env.define("length", new ALVar(core.proc_length, type.PROC));
	env.define("list", new ALVar(core.proc_list, type.PROC));
	env.define("index", new ALVar(core.proc_index, type.PROC));
	env.define("head", new ALVar(core.proc_head, type.PROC));
	env.define("tail", new ALVar(core.proc_tail, type.PROC));
	env.define("empty?", new ALVar(core.proc_empty, type.PROC));
	env.define("typeof", new ALVar(core.proc_typeof, type.PROC));
	env.define("error", new ALVar(core.proc_error, type.PROC_ENV));
	env.define("cell:lambda", new ALVar(core.proc_lambda, type.PROC_ENV));
	env.define("cell:macro", new ALVar(core.proc_macro, type.PROC_ENV));
	env.define("cell:fastmacro", new ALVar(core.proc_fastmacro, type.PROC_ENV));
	env.define("cell:lambda_args", new ALVar(core.proc_lambda_args, type.PROC));
	env.define("cell:lambda_body", new ALVar(core.proc_lambda_body, type.PROC));
	env.define("cell:lambda_env", new ALVar(core.proc_lambda_env, type.PROC));
	env.define("cell:proc", new ALVar(core.proc_proc, type.PROC));
	env.define("cell:proc_env", new ALVar(core.proc_proc_env, type.PROC_ENV));
	env.define("env:capture", new ALVar(environment.proc_capture, type.PROC_ENV));
	env.define("env:recapture", new ALVar(environment.proc_recapture, type.PROC));
	env.define("env:current", new ALVar(environment.proc_current, type.PROC_ENV));
	env.define("env:defined", new ALVar(environment.proc_defined, type.PROC_ENV));
	env.define("env:define", new ALVar(environment.proc_define, type.PROC_ENV));
	env.define("env:set!", new ALVar(environment.proc_set, type.PROC_ENV));
	env.define("env:new", new ALVar(environment.proc_new, type.PROC_ENV));
	env.define("env:get", new ALVar(environment.proc_get, type.PROC_ENV));
	env.define("env:getor", new ALVar(environment.proc_getor, type.PROC_ENV));
	env.define("env:rlookup", new ALVar(environment.proc_rlookup, type.PROC_ENV));
	env.define("env:dump", new ALVar(environment.proc_dump, type.PROC_ENV));
	env.define("string:split", new ALVar(strings.split, type.PROC));
	env.define("string:join", new ALVar(strings.join, type.PROC));
	env.define("string:substr", new ALVar(strings.substr, type.PROC));
	env.define("string:repeat", new ALVar(strings.repeat, type.PROC));
	env.define("file:exists", new ALVar(file.exists, type.PROC));
	env.define("file:path", new ALVar(file.path, type.PROC));
	env.define("file:read", new ALVar(file.read, type.PROC));
	env.define("debug:parse", new ALVar(debug.parse), type.PROC);
}
exports.add_stdlib = add_stdlib;
