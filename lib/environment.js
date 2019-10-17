/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * environment - an ALisp environment with parent
 */

"use strict";

var atoms = require('./atoms');
var errors = require('./errors');
var types = require('./types');
var KeyNotFoundError = errors.KeyNotFoundError;

var env_counter = 0;
exports.env_counter = () => env_counter;

function Environment (keys, values, parent) {
	this._id = ++env_counter;
	this.map = {};
	if (keys) {
		if (keys.type !== types.type.LIST) {
			// Single key capturing multiple values
			keys = new types.ALVar([keys], types.type.LIST);
			//console.error(" Values type: ", values, typeof values);
			values = [new types.ALVar(values, types.type.LIST)];
		}
		this.addRange(keys, values);
	}
	if((this.parent = parent) instanceof types.ALVar)
		this.parent = this.parent.value;
}

Environment.prototype.new_child = function () {
	return new Environment(undefined, undefined, this);
};

Environment.prototype.addRange = function (keys, values) {
	if (!Array.isArray(values)) throw new errors.TypeError("Expected values to be a simple array", values);
//console.error("addRange with keys length: ", keys.size(), ", values length: ", values.length);
	for (var i = 0; i < keys.size() && i < values.length; ++i) {
//console.error(keys.index(i).toString(), "=>", values[i].toString());
		this.define(keys.index(i), values[i]);
	}
};

Environment.prototype.get = function(key) {
	if (key instanceof types.ALVar)
		key = key.value;
	else if (typeof key !== 'number')
		key = atoms.declare(key).value;
    if (key in this.map)
        return this.map[key];
    if (this.parent)
        return this.parent.get(key);
    throw new KeyNotFoundError(atoms.toString(key), this);
};

// Reverse lookup, find what symbol is associated with given value.
// Returns a list of candidates.
Environment.prototype.rlookup = function(value) {
	var candidates = [];
	for(var key in this.map) {
		var innerValue = this.map[key];
		var atomId = Number(key);
		if(this.map[key] && this.map[key].equal(value))
			candidates.push(new types.ALVar(atomId, types.type.ATOM));
	}
	if (this.parent)
		candidates = candidates.concat(this.parent.rlookup(value));
	return candidates;
};

Environment.prototype.define = function (key, value) {
	if (key instanceof types.ALVar)
		key = key.value;
	else if (typeof key !== 'number')
		key = atoms.declare(key).value;
    return this.map[key] = value;
};

Environment.prototype.defined = function (key) {
	if (key instanceof types.ALVar)
		key = key.value;
	else if (typeof key !== 'number')
		key = atoms.declare(key).value;
    if (key in this.map)
		return true;
	if (this.parent)
		return this.parent.defined(key);
	return false;
};

Environment.prototype.set = function (key, value) {
	if (key instanceof types.ALVar)
		key = key.value;
	else if (typeof key !== 'number')
		key = atoms.declare(key).value;
    if (key in this.map)
        return this.map[key] = value;
    if (this.parent)
        return this.parent.set(key, value);
    throw new KeyNotFoundError(key, this);
};

exports.Environment = Environment;
