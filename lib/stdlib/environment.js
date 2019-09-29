/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * stdlib/environment - Environment manipulation
 */

"use strict";

var atoms = require('../atoms');
var runtime = require('../runtime');

exports.defined = (exps, env) => {
	runtime.check(exps, runtime.has_one_argument);
	var symbol = exps[0].toString();
	return atoms.bool(env.value.defined(symbol));
};
