/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * stdlib/strings - string functions
 */

"use strict";

var runtime = require('../runtime');
var types = require('../types');
var ALVar = types.ALVar, type = types.type;

// string:split(Seperator, String) -> list()
function split(args) {
	if (args.length !== 2) throw 'requires 2 arguments';
	var seperator = args[0].str();
	var str = args[1].str();
	return new ALVar(str.split(seperator), type.LIST);
}

function join(args) {
	if (args.length !== 2) throw 'requires 2 arguments';
	var seperator = args[0].str();
	return new ALVar(args[1].join(seperator), type.STRING);
}

exports.split = split;
exports.join = join;

exports.substr = (exps) => {
	runtime.check(exps, runtime.has_two_arguments);
	var str = exps[0].toString();
	var start = exps[1].value;
	var end = undefined;
	if (exps.length >= 3)
		end = exps[2].value;
	return new ALVar(str.substr(start, end), type.STRING);
};

exports.repeat = (exps) => {
	runtime.check(exps, runtime.has_two_arguments);
	var str = exps[0].toString();
	var count = exps[1].value;
	if(typeof count !== 'number')
		return str;
	return new ALVar(str.repeat(count), type.STRING);
};
