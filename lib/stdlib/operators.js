/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * stdlib/operators - Mathematical and comparison operators
 */

"use strict";

var atoms = require('../atoms');
var runtime = require('../runtime');
var types = require('../types');
var ALVar = types.ALVar, type = types.type;

function proc_plus(exps) {
	runtime.check(exps, runtime.has_arguments);
	var value = new ALVar(exps.shift()); // grab first
	while (exps.length > 0)
		value.value += exps.shift().coerce(value.type);
	return value;
}
exports.proc_plus = proc_plus;

function proc_minus(exps) {
	runtime.check(exps, runtime.has_arguments);
	var value = new ALVar(exps.shift()); // grab first
	while (exps.length > 0)
		value.value -= exps.shift().coerce(value.type);
	return value;
}
exports.proc_minus = proc_minus;

function proc_multiply(exps) {
	runtime.check(exps, runtime.has_arguments);
	var value = new ALVar(exps.shift()); // grab first
	while (exps.length > 0)
		value.value *= exps.shift().coerce(value.type);
	return value;
}
exports.proc_multiply = proc_multiply;

function proc_divide(exps) {
	runtime.check(exps, runtime.has_arguments);
	var value = new ALVar(exps.shift()); // grab first
	while (exps.length > 0)
		value.value /= exps.shift().coerce(value.type);
	return value;
}
exports.proc_divide = proc_divide;

function proc_equal(exps) {
	runtime.check(exps, runtime.has_two_arguments);
	return atoms.bool(exps[0].equal(exps[1]));
}
exports.proc_equal = proc_equal;

function proc_nequal(exps) {
	runtime.check(exps, runtime.has_two_arguments);
	return atoms.bool(!exps[0].equal(exps[1]));
}
exports.proc_nequal = proc_nequal;

exports.proc_not = (exps) => {
	runtime.check(exps, runtime.has_one_argument);
	return atoms.bool(!exps[0].equal(atoms.symbols.True));
};

exports.proc_lessthan = (exps) => {
	runtime.check(exps, runtime.has_two_arguments);
	var first = exps[0];
	var second = exps[1];
	return atoms.bool(first.value < second.value);
};

exports.proc_lessthanequal = (exps) => {
	runtime.check(exps, runtime.has_two_arguments);
	var first = exps[0];
	var second = exps[1];
	return atoms.bool(first.value <= second.value);
};
