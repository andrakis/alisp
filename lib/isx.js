/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * isx - library for checking if something is a given item
 */

"use strict";

var types = require('./types');

exports.isspace = function(c) {
	return ' '  === c ||
	       '\t' === c ||
	       '\r' === c ||
	       '\n' === c;
};

var ascii0 = '0'.charCodeAt(0);
var ascii9 = '9'.charCodeAt(0);
exports.isdigit = function(c) {
	if ('' === c) return false;
	if (typeof c === 'string')
		c = c.charCodeAt(0);
	return c >= ascii0 && c <= ascii9;
};

exports.isalvar = function (o, type) {
	if (o instanceof types.ALVar) {
		return type === undefined || o.type === type;
	}
	return false;
};