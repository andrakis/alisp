/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * version - version information
 */

"use strict";

var versionMajor = "0";
var versionMinor = "2";
var versionBuild = "2";
var platformMajor = "0";
var platformMinor = "3";
var platformName = 'NodeJS';

var versionStr = [versionMajor, versionMinor, versionBuild].join('.');
versionStr += ' platform ' + [platformMajor, platformMinor].join('.') + ' ' + platformName;
exports.version = versionStr;
