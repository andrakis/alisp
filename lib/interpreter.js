/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * interpreter - contains the interpreters for ALisp
 *
 * Simple Interpreter: a simple recursive eval function that supports
 *                     tail recursion.
 * 
 * Frame Interpreter:  advanced evaluator using frames
 *  A frame is the current state of interpretation. It is a state machine
 *  frame, giving the context of the current invocation.
 *  New calls are in new frames, and as each frame finishes it signals to
 *  its parent frame, which puts the data in the appropriate place and moves
 *  onto the next state.
 */

"use strict";

var atoms = require('./atoms');
var { Environment } = require('./environment');
var errors = require('./errors');
var types = require('./types');
var ALVar = types.ALVar, type = types.type;

var Nil = atoms.symbols.Nil, True = atoms.symbols.True, False = atoms.symbols.False;
var _quote = atoms.declare("quote").value, _if = atoms.declare("if").value, _define = atoms.declare("define").value;
var _set = atoms.declare("set!").value, _lambda = atoms.declare("lambda").value, _macro = atoms.declare("macro").value;
var _fastmacro = atoms.declare("fastmacro").value, _begin = atoms.declare("begin").value, _next = atoms.declare("next").value;

function SimpleEval(x, env) {
	do {
		switch (x.type) {
			case type.ATOM:
				return env.value.get(x.value);
			case type.STRING:
			case type.NUMBER:
				return x;
		}
		if (x.empty()) return Nil;
		var sym = x.head();
		var isNext = false;
		if (sym.type === type.ATOM) {
			var id = sym.value;
			if (id === _quote) {          // (quote exp)
				return x.index(1);
			} else if (id === _if) {      // (if test conseq [alt])
				var test = x.index(1);
				var conseq = x.index(2);
				var alt = x.index(3) || Nil;
				var result = SimpleEval(test, env);
				x = result === False ? alt : conseq;
				continue;
			} else if (id === _define) {  // (define var exp) - create new variable
				return env.value.define(x.index(1), SimpleEval(x.index(2), env));
			} else if (id === _set) {     // (set! var exp) - update existing variable
				return env.value.set(x.index(1), SimpleEval(x.index(2), env));
			} else if (id === _lambda) {  // (lambda (var*) exp)
				return types.lambda(x.index(1), x.index(2), env);
			} else if (id === _macro) {   // (macro (var*) exp)
				return types.macro(x.index(1), x.index(2), env);
			} else if (id === _fastmacro) { // (fastmacro (var*) exp)
				return types.fastmacro(x.index(1), x.index(2), env);
			} else if (id === _begin) {   // (begin exp*)
				var start = 1; // skip begin
				var end = x.size() - 1; // run to end - 1
				for (var i = start; i < end; ++i)
					SimpleEval(x.index(i), env);
				x = x.index(end);
				continue;
			} else if (id === _next) {    // (next fun args)
				isNext = true;
				x = x.tail();
			}
		}

		// (proc exp*)
		var proc = SimpleEval(x.index(0), env);
		var exps = [];
		if (proc.type === type.MACRO || proc.type === type.FASTMACRO)
			exps = x.tail().value;
		else {
			// (map (tail x) (lambda (y) (eval y env)))
			for (var l = 1; l < x.size(); ++l) {
				exps.push(SimpleEval(x.index(l), env));
			}
		}
		switch (proc.type) {
			case type.LAMBDA:
				if (isNext) {
					//env = proc.index(types.lambda_env);
					env.value.recapture(proc.index(types.lambda_args), exps);
				} else
					env = new ALVar(new Environment(proc.index(types.lambda_args), exps, proc.index(types.lambda_env).value), type.ENV);
				x = proc.index(types.lambda_body);
				continue;
			case type.MACRO:
				var env2 = new ALVar(new Environment(proc.index(types.lambda_args), exps, proc.index(types.lambda_env).value), type.ENV);
				x = SimpleEval(proc.index(types.lambda_body), env2);
				continue;
			case type.FASTMACRO:
				var env2 = proc.index(types.lambda_env);
				env2.value.recapture(proc.index(types.lambda_args), exps);
				x = SimpleEval(proc.index(types.lambda_body), env2);
				continue;
			case type.PROC:
				return proc.value(exps);
			case type.PROC_ENV:
				return proc.value(exps, env);
			default:
				console.error(proc.toString());
				throw new errors.RuntimeError("Not an executable cell", x);
		}
	} while (true);
}
exports.SimpleEval = SimpleEval;

function fire(state, evt, data) {
	var cb;
	if((cb = state.events[evt]) !== null)
		cb(data);
}

exports.execute = function(code, env) {
	// Did I just reinvent event emitters?
	return (function(state) {
		state.on = function(evt, cb) { state.events[evt] = cb; return state; };
		state.start = function() {
			try {
				if(!(code instanceof ALVar)) throw new errors.RuntimeError("Not an ALVar");
				var result = SimpleEval(state.code, state.env);
				fire(state, 'success', result);
			} catch (e) {
				fire(state, 'error', e);
			}
		};
		return state;
	})({
		events: {
			'error': null,
			'success': null
		},
		code: code,
		env: env
	});
};
