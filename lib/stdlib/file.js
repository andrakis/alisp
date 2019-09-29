/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * stdlib/file - contains functions for operating on files
 */

"use strict";

const fs = require('fs');
var atoms = require('../atoms');
var runtime = require('../runtime');
var types = require('../types');
var ALVar = types.ALVar;

var search_paths = [
	"",
	"./",
	"./lisp/"
];

var native_exists = (file) => fs.existsSync(file);

exports.exists = (file) => {
	return atoms.bool(native_exists(file));
};

exports.path = (args) => {
	runtime.check(args, runtime.has_one_argument);
	var file = args[0].toString();
	for(var i = 0; i < search_paths.length; ++i) {
		var fullpath = search_paths[i] + file;
		if (native_exists(fullpath))
			return new ALVar(fullpath, types.type.STRING);
	}
	return args[0];
};

exports.read = (args) => {
	runtime.check(args, runtime.has_one_argument);
	var file = exports.path([args[0].toString()]).toString();
	var contents = fs.readFileSync(file, 'utf8');
	return new ALVar(contents, types.type.STRING);
};
