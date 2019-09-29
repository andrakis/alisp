/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * parser - contains the ALisp parser
 */

"use strict";

var atoms = require('./atoms');
var errors = require('./errors');
var isx = require('./isx');
var isspace = isx.isspace, isdigit = isx.isdigit;
var types = require('./types');
var ALVar = types.ALVar, type = types.type;

function startOf (str) {
	if (str === '') return '';
	return str[0];
}

function endOf (str) {
	if (str === '') return '';
	return str[str.length - 1];
}

function tokenise(str) {
	var tokens = [];
	var s = 0, t; // string counters, use s for most and t when required
	while(s < str.length) {
		while (isspace(str[s]))                    // Skip whitespace
			++s;
		if (str.substr(s, 2) === ';;')             // Skip comment lines
			while(s < str.length && str[s] !== '\n' && str[s] !== '\r')
				++s;
		else if (str[s] === '(' || str[s] === ')') // List open or close
			tokens.push(str[s++] === '(' ? '(' : ')');
		else if (str[s] === '"') {                 // "string" in quotes
			t = s;
			var escape = 0;
			do {
				++t;
				if (escape !== 0) escape--;
				if (str[t] === '\\') escape = 2;
			} while (t < str.length && (escape !== 0 || str[t] !== '"'));
			++t;
			tokens.push(str.substr(s, t - s));
			s = t;
		} else {                                   // A generic token
			t = s;
			while (t < str.length && !isspace(str[t]) && str[t] !== '(' && str[t] !== ')')
				++t;
			tokens.push(str.substr(s, t - s));
			s = t;
		}
	}
	return tokens;
}

function atom (token) {
	if (startOf(token) === '"' && endOf(token) === '"')
		return new ALVar(token.substr(1, token.length - 2), type.STRING);
	if (isdigit(token[0]) || token.length > 1 && token[0] === '-' && isdigit(token[1])) {
		// coerce token to a number by calling Number class
		return new ALVar(Number(token), type.NUMBER);
	}
	return atoms.declare(token);
}

function read_from(tokens) {
	if (tokens.length === 0) throw new errors.ParserError("Missing opening token");

	var token = tokens.shift();
	if (token === "(") {
		var cells = new ALVar([], type.LIST);
		while (tokens[0] !== ")") {
			cells.push(read_from(tokens));
			if (tokens.length === 0) throw new errors.ParserError("Missing closing )");
		}
		if (tokens.length === 0) throw new errors.ParserError("Missing closing )");
		tokens.shift(); // discard closing )
		return cells;
	}
	return atom(token);
}

function read(str) {
	var tokens = tokenise(str);
	return read_from(tokens);
}

exports.read = read;
exports.detail = {
	atom: atom,
	read_from: read_from,
	tokenise: tokenise
};
