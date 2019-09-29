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
var environment = require('./stdlib/environment');
var file = require('./stdlib/file');
var operators = require('./stdlib/operators');
var strings = require('./stdlib/strings');

// Export sub items
exports.libs = {
	core: core,
	conio: conio,
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
	env.define("not", new ALVar(operators.proc_not, type.PROC));
	env.define("length", new ALVar(core.proc_length, type.PROC));
	env.define("list", new ALVar(core.proc_list, type.PROC));
	env.define("head", new ALVar(core.proc_head, type.PROC));
	env.define("tail", new ALVar(core.proc_tail, type.PROC));
	env.define("empty?", new ALVar(core.proc_empty, type.PROC));
	env.define("env:defined", new ALVar(environment.defined, type.PROC_ENV));
	env.define("string:split", new ALVar(strings.split, type.PROC));
	env.define("string:join", new ALVar(strings.join, type.PROC));
	env.define("file:exists", new ALVar(file.exists, type.PROC));
	env.define("file:path", new ALVar(file.path, type.PROC));
	env.define("file:read", new ALVar(file.read, type.PROC));
}
exports.add_stdlib = add_stdlib;
