/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * errcodes - error codes and information
 */

"use strict";

// Also see accompanying items in messages
var codes = {
	NONE: 0,
	MISSING_FILE: 1,
	NOTHING_DONE: 2,
};

var messages = {};
messages[codes.NONE] = "No error";
messages[codes.MISSING_FILE] = "Missing file";
messages[codes.NOTHING_DONE] = "No useful arguments given";

function throwError (errCode) {
	console.error('Thrown: ' + errCode + ': ' + messages[errCode]);
	returnError(errCode);
}

function returnError(errCode) {
	process.exit(errCode);
}

exports.codes = codes;
exports.messages = messages;
exports.throwError = throwError;
exports.returnError = returnError;
