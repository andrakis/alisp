/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * stdlib/core - Core functions
 */

"use strict";

var atoms = require('../atoms');
var runtime = require('../runtime');
var types = require('../types');
var ALVar = types.ALVar, type = types.type;

exports.proc_length = (exps) => {
	runtime.check(exps, runtime.has_one_argument);
	var first = exps[0];
	return new ALVar(first.size(), type.NUMBER);
};

exports.proc_list = (exps) => new ALVar(exps, type.LIST);

exports.proc_head = (exps) => {
	runtime.check(exps, runtime.has_one_argument);
	return exps[0].head();
};

exports.proc_tail = (exps) => {
	runtime.check(exps, runtime.has_one_argument);
	return exps[0].tail();
};

exports.proc_empty = (exps) => {
	runtime.check(exps, runtime.has_one_argument);
	return atoms.bool(exps[0].empty());
};
