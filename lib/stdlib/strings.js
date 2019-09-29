/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * stdlib/strings - string functions
 */

"use strict";

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
