/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 */

"use strict";

var alisp = require('./lib/alisp');

// Export everything
for (var key in alisp) {
	exports[key] = alisp[key];
}

// Check if invoked directly, or required.
// If invoked directly, call alisp.main.
if (typeof module !== 'undefined' && !module.parent) {
	alisp.main();
}
