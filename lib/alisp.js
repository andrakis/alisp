/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * alisp - main importer of alisp modules; also contains main()
 */

"use strict";

exports.atoms = require('./atoms');
exports.environment = require('./environment');
exports.errcodes = require('./errcodes');
exports.errors = require('./errors');
exports.interpreter = require('./interpreter');
exports.isx = require('./isx');
exports.parser = require('./parser');
exports.process = require('./process');
exports.repl = require('./repl');
exports.stdlib = require('./stdlib');
exports.tests = require('./tests');
exports.types = require('./types');
exports.version = require('./version').version;

var tests = exports.tests, errcodes = exports.errcodes;
var version = exports.version;

function readArguments (argc, argv) {
	var acceptArguments = true; // overridden with --
	var state = {
		help: false,
		interactive: false,
		show_timing: false,
		timing: {},
		tests: false,
		debug: false,
		file: "",
		args: [],
		did_anything: false
	};

	var i = 2; // skip node.js and index.js
	while (i < argc) {
		var a = argv[i];
		if (acceptArguments) {
			if ('--help' === a || '-h' === a)
				state.help = true;
			else if ('--interactive' === a || '-i' === a)
				state.interactive = true;
			else if ('--timing' === a || '-t' === a)
				state.show_timing = true;
			else if ('--tests' === a || '-T' === a)
				state.tests = true;
			else if ('--debug' === a || '-d' === a)
				state.debug = true;
			else if ('--' === a) {
				if (state.file === "")
					throw 'filename must be specified before end of options';
				acceptArguments = false;
			} else if (state.file === "") {
				state.file = a;
				acceptArguments = false;
			} else
				state.args.push(a);
		} else  state.args.push(a);
		++i;
	}

	// Convert args to ALVar's
	try {
		state.args = state.args.map((a) => exports.parser.read(a));
	} catch (e) {
		console.error("Failed to parse arguments");
		throw e;
	}
	state.args = new exports.types.ALVar(state.args, exports.types.type.LIST);

	return state;
}

function showHelp(argc, argv) {
	console.error("ALisp v " + version);
	console.error("");
	console.error("Usage:");
	console.error(argv[1] + " [-i] [-t] [-T] [-d] [file] [args...]");
	console.error("Arguments must come before [file].");
	console.error("Anything after [file] is passed to the script being run.");
	console.error("\t-i             Interactive mode (REPL)");
	console.error("\t-t             Display timing information");
	console.error("\t-T             Run tests");
	console.error("\t-d             Debug mode");
	console.error("\tfile           File to parse and execute");
	console.error("\targs...        Arguments to pass to script");
}

var state;

function main_exec_error (e) {
	state.timing['execute'] = ts() - state.timing['execute'];
	console.error(e.stack);
	// TODO: are all threads finished?
	if (state.interactive) {
		repl();
	} else {
		final_cleanup();
		process.exit(2);
	}
}
function main_exec_success (result) {
	state.timing['execute'] = ts() - state.timing['execute'];
	console.log(result.toString());
	// TODO: are all threads finished?
	if (state.interactive) {
		repl();
	} else {
		final_cleanup();
		process.exit(0);
	}
}

function final_cleanup () {
	state.envCell = null;
	state.timing['runtime'] = ts() - state.timing['start'];
	delete state.timing['start'];
	if (state.debug) {
		console.error("(debug) Environment counter:", exports.environment.env_counter());
	}
	if (state.show_timing) {
		console.error("(timing) Timing information:");
		for(var name in state.timing)
			console.error(`\t${name}: ${state.timing[name]}ms`);
	}
}

var ts = () => new Date().getTime();

function repl_close () {
	state.timing['repl'] = ts() - state.timing['repl'];
	final_cleanup();
}

function repl () {
	state.did_anything = true;
	state.timing['repl'] = ts();
	exports.repl.REPL(state.envCell)
		.on('close', repl_close)
		.start();
}

exports.main = function() {
	var argc = 0, argv = ['alisp.js'];
	if (typeof process !== 'undefined') {
		argc = process.argv.length;
		argv = process.argv;
	}

	state = readArguments(argc, argv);

	var lbl;
	state.timing[lbl = 'start'] = ts();
	state.timing[lbl = 'create environment'] = state.timing['start'];
	var env = new exports.environment.Environment();
	exports.stdlib.add_stdlib(env);
	state.envCell = new exports.types.ALVar(env);
	state.timing[lbl] = ts() - state.timing[lbl];

	if (state.help) {
		state.did_anything = true;
		showHelp(argc, argv);
		errcodes.returnError(errcodes.codes.NOTHING_DONE);
	}

	if (state.tests) {
		state.did_anything = true;
		state.timing[lbl = 'run tests'] = ts();
		tests.test();
		state.timing[lbl] = ts() - state.timing[lbl];
	}
	
	if (state.file) {
		state.did_anything = true;
		env.define('argv', state.args);
		// Find file
		var file = exports.stdlib.libs.file.path([state.file]).toString();
		try {
			var content, parsed;
			state.timing[lbl = 'parse'] = ts();
			// Read file
			content = exports.stdlib.libs.file.read([file]).toString();
			// Parse file
			parsed = exports.parser.read(content);
			state.timing[lbl] = ts() - state.timing[lbl];
			state.timing[lbl = 'execute'] = ts();
			// Execute content
			exports.interpreter.execute(parsed, state.envCell)
				.on('error', main_exec_error)
				.on('success', main_exec_success)
				.start();
		} catch (e) {
			console.error(e.stack);
			final_cleanup();
			process.exit(1);
		}
	}

	if (!state.file && state.interactive) {
		repl();
	}

	if (false === state.did_anything) {
		showHelp(argc, argv);
		errcodes.returnError(errcodes.codes.NOTHING_DONE);
	}
};
