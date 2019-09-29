/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * tests - basic tests of the interpreter
 */

"use strict";

var alisp; // will be filled in later

function test () {
	alisp = require('../index');
	var ALVar = alisp.types.ALVar, type = alisp.types.type;
	var Environment = alisp.environment.Environment;

	var code = "(begin " +
		"(define fac (lambda (n) (fac/2 n 1))) " +
		"(define fac/2 (lambda (n a) " +
			"(if (= 1 n) a (fac/2 (- n 1) (* n a)))))" +
		"(define x 10) " +
		"(print \"Fac\" x (fac x)))";
	var code_cell = alisp.parser.read(code);
	var env = new ALVar(new Environment(), type.ENV);
	alisp.stdlib.add_stdlib(env.value);
	var result = alisp.interpreter.SimpleEval(code_cell, env);
	console.log(result.toString());
}

exports.test = test;

