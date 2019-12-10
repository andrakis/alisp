/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * stdlib/environment - Environment manipulation
 */

"use strict";

var atoms = require('../atoms');
var environment = require('../environment');
var runtime = require('../runtime');
var types = require('../types');

// -spec (env:new) -> env().
// -spec (env:new Parent::env()) -> env().
exports.proc_new = (exps, env) => {
	var parentEnv = env.value;
	if (exps.length > 0)
		parentEnv = exps[0];
	if (parentEnv == atoms.symbols.Nil)
		return new types.ALVar(new environment.Environment());
	return new types.ALVar(parentEnv.new_child(), types.type.ENV);
};

// -spec (env:capture Keys::[atom()] Values::[any()] Parent::env()) -> env().
exports.proc_capture = (exps, env) => {
	runtime.check(exps, runtime.has_two_arguments);
	var keys = exps[0];
	var values = exps[1].value;
	var parent = env;
	if (exps.length >= 3)
		parent = exps[2];
	if (parent instanceof types.ALVar)
		parent = parent.value;
	var created = new environment.Environment(keys, values, parent);
	return new types.ALVar(created, types.type.ENV);
};

exports.proc_recapture = (exps) => {
	runtime.check(exps, runtime.has_three_arguments);
	var env = exps[0];
	var keys = exps[1];
	var values = exps[2].value;
	env.value.recapture(keys, values);
	return env;
};

exports.proc_current = (exps, env) => {
	return env;
};

exports.proc_defined = (exps, env) => {
	runtime.check(exps, runtime.has_arguments);
	var symbol = exps[0].toString();
	var targetEnv = env;
	if (exps.length >= 2)
		targetEnv = exps[1].value;
	if (targetEnv instanceof types.ALVar)
		targetEnv = targetEnv.value;
	return atoms.bool(targetEnv.defined(symbol));
};

exports.proc_get = (exps, env) => {
	runtime.check(exps, runtime.has_arguments);
	var symbol = exps[0];
	var targetEnv = env.value;
	if (exps.length >= 2)
		targetEnv = exps[1].value;
	if (targetEnv instanceof types.ALVar)
		targetEnv = targetEnv.value;
	return targetEnv.get(symbol);
};

exports.proc_getor = (exps, env) => {
	runtime.check(exps, runtime.has_arguments);
	var symbol = exps[0];
	var defaultVal = exps[1];
	var targetEnv = env.value;
	if (exps.length >= 2)
		targetEnv = exps[2].value;
	if (targetEnv instanceof types.ALVar)
		targetEnv = targetEnv.value;
	//console.log("!!getor", symbol.toString(), defaultVal.toString());
	if (!targetEnv.defined(symbol))
		return defaultVal;
	return targetEnv.get(symbol);
};

exports.proc_define = (exps, env) => {
	runtime.check(exps, runtime.has_two_arguments);
	var symbol = exps[0].toString();
	var value = exps[1];
	var targetEnv = env;
	if (exps.length >= 3) // provided a targetEnv
		targetEnv = exps[2];
	targetEnv.value.define(symbol, value.clone());
	return value.clone();
};

exports.proc_set = (exps, env) => {
	runtime.check(exps, runtime.has_two_arguments);
	var symbol = exps[0].toString();
	var value = exps[1];
	var targetEnv = env;
	if (exps.length >= 3) // provided a targetEnv
		targetEnv = exps[2];
	targetEnv.value.set(symbol, value.clone());
	return value.clone();
};

exports.proc_rlookup = (exps, env) => {
	runtime.check(exps, runtime.has_one_argument);
	var candidates = env.value.rlookup(exps[0]);
	return new types.ALVar(candidates, types.type.LIST);
};

function envDepth (env) {
	var count = 1;
	var i = env;
	do { ++count; } while((i = i.parent));
	return count;
}
exports.proc_dump = (exps, env) => {
	var target = env.value;
	if (exps.length > 0)
		target = exps[0];
	if (target instanceof types.ALVar)
		target = target.value;
	var depth = envDepth(target);
	do {
		for(var key in target.map) {
			var readable = atoms.toString(Number(key));
			var value = target.map[key];
			console.error("-".repeat(depth) + " " + readable + " -> " + value.toString());
		}
		depth--;
		target = target.parent;
	} while(target);
	return atoms.symbols.Nil;
};
