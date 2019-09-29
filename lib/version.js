/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * version - version information
 */

"use strict";

var versionMajor = "0";
var versionMinor = "1";
var versionBuild = "1";
var platformMajor = "0";
var platformMinor = "1";
var platformName = 'NodeJS';

var versionStr = [versionMajor, versionMinor, versionBuild].join('.');
versionStr += ' platform ' + [platformMajor, platformMinor].join('.') + ' ' + platformName;
exports.version = versionStr;
