/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * types - ALisp types
 */

"use strict";

var atoms = require('./atoms');
var environment = require('./environment');
var errors = require('./errors');

var enum_type = {
	NONE: 0,
	ATOM: 1,
	STRING: 2,
	NUMBER: 3,
	LIST: 4,
	LAMBDA: 5,
	MACRO: 6,
	PROC: 7,
	PROC_ENV: 8,
	ENV: 9,
	readable: (t) => {
		for(var key in enum_type)
			if (t === enum_type[key])
				return key;
		throw 'readable(' + t + '): not found';
	}
};

function ALVar (value, type) {
	this.value = value;
	if (value instanceof ALVar) {
		this.value = value.value;
		this.type = value.type;
	} else if (type === undefined) {
		// determine type based on value
		this.type = this.determineValueType();
	} else {
		//XXX removed: coerce to given type
		//this.value = value;
		//this.type = this.determineValueType();
		this.value = value;
		this.type = type;
	}
}
ALVar.prototype.determineValueType = function () {
	if (this.value instanceof environment.Environment)
		return enum_type.ENV;
	else if (typeof this.value === 'string')
		return enum_type.STRING;
	else if (typeof this.value === 'number')
		return enum_type.NUMBER;
	else if (Array.isArray(this.value))
		return enum_type.LIST;
	else if (typeof this.value === 'function')
		return enum_type.PROC;
	else if (this.value === undefined)
		return enum_type.NONE;
	throw new errors.RuntimeError("Could not determine value type", this.value);
};

ALVar.prototype.coerce = function(type) {
	if (type === this.type)
		return this.value;
	else if (type === enum_type.STRING)
		return this.toString();
	else if (type === enum_type.NUMBER || type === enum_type.ATOM)
		return Number(this.value);
	else if (type === enum_type.LIST)
		return [this.value];
	throw new errors.RuntimeError('could not coerce from ' + enum_type.readable(this.type) + ' to ' + enum_type.readable(type), this);
};

ALVar.prototype.equal = function (other) {
	switch (this.type) {
		case enum_type.STRING: return this.toString() === other.toString(); 
		case enum_type.NUMBER:
			if (other.type === this.type) return this.value === other.value;
			// compare as strings
			return this.toString() === other.toString();
		case enum_type.NONE: return other.type === enum_type.NONE;
		case enum_type.ATOM:
		case enum_type.PROC:
		case enum_type.PROC_ENV: 
		case enum_type.ENV:  return other.type === this.type && this.value === other.value;
		case enum_type.LIST:
		case enum_type.LAMBDA:
		case enum_type.MACRO:
			if (other.type !== this.type) return false;
			if (other.length() !== this.length()) return false;
			for (var i = 0; i < this.length(); ++i)
				if (this.index(i).notEqual(other.index(i)))
					return false;
			return true;
		default:
			throw new errors.RuntimeError("Unknown type: " + this.type, this);
	}
};
ALVar.prototype.notEqual = function (other) {
	return !this.equal(other);
};

ALVar.prototype.toString = function () {
	switch (this.type) {
		case enum_type.NONE: return "(none)";
		case enum_type.ATOM: return atoms.toString(this.value);
		case enum_type.STRING:
		case enum_type.NUMBER: return this.value.toString();
		case enum_type.PROC: return "#proc";
		case enum_type.PROC_ENV: return "#procenv";
		case enum_type.ENV: return "#env";
		case enum_type.LIST:
		case enum_type.LAMBDA:
		case enum_type.MACRO:
			var open = "(";
			if (this.type === enum_type.LAMBDA) open = "(lambda ";
			else if (this.type === enum_type.MACRO) open = "(macro ";
			var str = this.value.map((cell) => cell.toString()).join(" ");
			return open + str + ")";
		default:
			throw new errors.RuntimeError("Unknown type: " + this.type, this);
	}
};
ALVar.prototype.isListType = function () {
	return this.type === enum_type.LIST ||
		this.type === enum_type.LAMBDA ||
		this.type === enum_type.MACRO;
};

// List behaviours
function list_check(cell) {
	if (!cell.isListType()) throw new errors.RuntimeError("Not a list type", cell);
}
ALVar.prototype.head = function () {
	return this.notEmpty() ? this.value[0] : atoms.symbols.Nil;
};
ALVar.prototype.tail = function () {
	return this.notEmpty() ? new ALVar(this.value.slice(1), enum_type.LIST) : atoms.symbols.Nil;
};
ALVar.prototype.index = function (idx) {
	if (!this.isListType()) return atoms.symbols.Nil;
	if (idx < this.value.length) {
		return this.value[idx];
	}
	return atoms.symbols.Nil;
};
ALVar.prototype.empty = function () {
	return this.isListType() && this.value.length === 0;
};
ALVar.prototype.notEmpty = function () {
	return this.isListType() && this.value.length !== 0;
};
ALVar.prototype.size = function () {
	return this.isListType() ? this.value.length : 0;
};
ALVar.prototype.push = function (value) {
	list_check(this);
	this.value.push(value);
};
ALVar.prototype.shift = function () {
	list_check(this);
	return this.value.shift();
};

ALVar.prototype.repr = function () {
	return "TODO";
};

exports.ALVar = ALVar;
exports.type = enum_type;
