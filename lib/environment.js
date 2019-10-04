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

function Environment (keys, values, parent) {
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
	this.parent = parent;
}

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
