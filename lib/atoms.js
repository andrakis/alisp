/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * atoms - manages the atoms list
 */

"use strict";

var errors = require('./errors');
var types = require('./types');
var ALVar = types.ALVar, type = types.type;

var atom_to_string = {}; // key: atom (number)
var string_to_atom = {}; // key: string
var atom_to_cell = {};   // key: atom (number)
var atom_counter = 0;

function toString(id) {
	if (typeof id === 'number')
		return toStringFromNumber(id);
	return id;
}

function toStringFromNumber(id) {
	if (id in atom_to_string)
		return atom_to_string[id];
	throw new errors.AtomNotFoundError(id, atom_to_string);
}

function exists(id) {
	if (typeof id === 'number')
		return existsFromNumber(id);
	return existsFromString(id);
}

function existsFromNumber(id) {
	for (var key in atom_to_string)
		if (key === id)
			return true;
	return false;
}

function existsFromString(id) {
	if (string_to_atom[id] !== undefined)
		return true;
	return false;
}

function get(id) {
	if (typeof id === 'number')
		return getFromNumber(id);
	return getFromString(id);
}

function getFromNumber(id) {
	if (!existsFromNumber)
		throw new excepts.AtomNotFoundError(id, atom_to_cell);
	return atom_to_cell[id];
}

function getFromString(id) {
	if (!existsFromString(id))
		throw new excepts.AtomNotFoundError(id, atom_to_cell);
	return atom_to_cell[string_to_atom[id]];
}

function declare(name) {
	if (exists(name))
		return get(name);
	var id = atom_counter++;
	var atom = new ALVar(id, type.ATOM);
	atom_to_cell[id] = atom;
	atom_to_string[id] = name;
	string_to_atom[name] = id;
	return atom;
}

function count() { return atom_counter; }

var Nil = declare("nil"), True = declare("true"), False = declare("false");

exports.toString = toString;
exports.exists = exists;
exports.get = get;
exports.declare = declare;
exports.count = count;
exports.detail = {
	existsFromNumber: existsFromNumber,
	existsFromString: existsFromString,
	toStringFromNumber: toStringFromNumber,
};
exports.symbols = {
	Nil: Nil,
	True: True,
	False: False
};
exports.bool = (b) => b ? True : False;
