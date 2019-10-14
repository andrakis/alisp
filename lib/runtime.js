/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * runtime - provides runtime checks and errors
 */

"use strict";

var errors = require('./errors');

exports.check = function(args, check) {
	var dat = check(args);
	var result = dat[0];
	var message = dat[1];
	if(!result) throw new errors.RuntimeError(message, args);
};

exports.has_arguments = (args) => [args.length > 0, "Requires at least one argument"];
exports.has_one_argument = (args) => [args.length === 1, "Requires at exactly one argument"];
exports.has_two_arguments = (args) => [args.length > 1, "Requires at least two arguments"];
