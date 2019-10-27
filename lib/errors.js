/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * errors - errors to be thrown and caught
 */

"use strict";

var util = require('util');

function AtomNotFoundError(key, atoms) {
    Error.captureStackTrace(this, this.constructor);
    this.name = this.constructor.name;
    this.message = `Atom not found: ${key}`;
    this.atoms = atoms;
}
exports.AtomNotFoundError = AtomNotFoundError;

function KeyNotFoundError(key, source) {
    Error.captureStackTrace(this, this.constructor);
    this.name = this.constructor.name;
    this.message = `Key not found: ${key} in instance ${source._id}`;
    this.source = source;
}
exports.KeyNotFoundError = KeyNotFoundError;

function ParserError(message) {
    Error.captureStackTrace(this, this.constructor);
    this.name = this.constructor.name;
	this.message = message;
}
exports.ParserError = ParserError;

function RuntimeError(message, source) {
    Error.captureStackTrace(this, this.constructor);
    this.name = this.constructor.name;
	this.message = message;
    this.source = source;
}
exports.RuntimeError = RuntimeError;

function ThrownError(message, source) {
    Error.captureStackTrace(this, this.constructor);
    this.name = this.constructor.name;
	this.message = message;
    this.source = source;
}
exports.ThrownError = ThrownError;

// An expected type was not provided - indicates a logic error.
function TypeError(message, source) {
    Error.captureStackTrace(this, this.constructor);
    this.name = this.constructor.name;
	this.message = message;
    this.source = source;
}
exports.TypeError = TypeError;

util.inherits(AtomNotFoundError, Error);
util.inherits(KeyNotFoundError, Error);
util.inherits(ParserError, Error);
util.inherits(RuntimeError, Error);
util.inherits(ThrownError, Error);
util.inherits(TypeError, Error);

