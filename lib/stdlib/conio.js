/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * stdlib/conio - Console IO
 */

"use strict";

var atoms = require('../atoms');

function proc_print(exps) {
	var msg = exps.map((cell) => cell.toString()).join(" ");
	console.log(msg);
	return atoms.symbols.Nil;
}
exports.proc_print = proc_print;
