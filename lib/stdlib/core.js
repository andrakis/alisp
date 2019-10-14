/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * stdlib/core - Core functions
 */

"use strict";

var atoms = require('../atoms');
var errors = require('../errors');
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

exports.proc_typeof = (exps) => {
	runtime.check(exps, runtime.has_one_argument);
	var readable = type.readable(exps[0].type);
	readable = readable.toLowerCase();
	return atoms.declare(readable);
};

exports.proc_error = (exps, env) => {
	runtime.check(exps, runtime.has_one_argument);
	throw new errors.ThrownError(exps[0].toString());
};

exports.proc_lambda = (exps, env) => {
	runtime.check(exps, runtime.has_two_arguments);
	var args = exps[0];
	var body = exps[1];
	var targetEnv = env;
	if (exps.length >= 3)
		targetEnv = exps[2];
	return types.lambda(args, body, targetEnv);
};

exports.proc_lambda_args = (exps) => {
	runtime.check(exps, runtime.has_one_argument);
	var lm = exps[0];
	if (!lm.isListType())
		throw new errors.TypeError('Requires a list type', lm);
	return lm.index(types.lambda_args);
};

exports.proc_lambda_body = (exps) => {
	runtime.check(exps, runtime.has_one_argument);
	var lm = exps[0];
	if (!lm.isListType())
		throw new errors.TypeError('Requires a list type', lm);
	return lm.index(types.lambda_body);
};

exports.proc_lambda_env = (exps) => {
	runtime.check(exps, runtime.has_one_argument);
	var lm = exps[0];
	if (!lm.isListType())
		throw new errors.TypeError('Requires a list type', lm);
	return lm.index(types.lambda_env);
};

exports.proc_macro = (exps, env) => {
	runtime.check(exps, runtime.has_two_arguments);
	var args = exps[0];
	var body = exps[1];
	var targetEnv = env;
	if (exps.length >= 3)
		targetEnv = exps[2];
	return types.macro(args, body, targetEnv);
};

exports.proc_index = (exps, env) => {
	runtime.check(exps, runtime.has_two_arguments);
	var list = exps[0];
	var index = exps[1];
	return list.index(index);
};

exports.proc_proc = (exps) => {
	runtime.check(exps, runtime.has_two_arguments);
	var proc = exps[0];
	var args = exps[1].value;
	return proc.value(args);
};

exports.proc_proc_env = (exps, env) => {
	runtime.check(exps, runtime.has_two_arguments);
	var proc = exps[0];
	var args = exps[1].value;
	var targetEnv = env;
	if (exps.length >= 3)
		targetEnv = exps[2];
	return proc.value(args, targetEnv);
};
