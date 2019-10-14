/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * stdlib/debug - contains debugging functions
 */

"use strict";

var atoms = require('../atoms');
var errors = require('../errors');
var runtime = require('../runtime');
var types = require('../types');
var parser = require('../parser');
var ALVar = types.ALVar, type = types.type;

//parsed = exports.parser.read(content);
exports.parse = (exps) => {
	runtime.check(exps, runtime.has_one_argument);
	return parser.read(exps[0].toString());
};
