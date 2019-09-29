/**
 * ALisp
 *  - A Lisp interpreter and platform in Node.js/Javascript
 *
 * process - lightweight processes
 */

"use strict";

// Process ID
// Structure:
//  <ProcessID, NodeID, GlobalID>
//
// ProcessID is unique among Nodes.
// NodeID is unique among Globals.
// GlobalID is unique and a number.
//
// ProcessID 0 is reserved, and serves as "this process id".
// NodeID 0 is reserved, and serves as "this node id".
// GlobalID 0 is reserved, and serves as "this global id".
//
// Process messages are forwarded first to the appropriate global id,
// and from there to the appropriate node id, which will be running the
// target process id.
function ProcessID (pid, nid, gid) {
	this.process_id = pid;
	this.node_id = nid;
	this.global_id = gid;
}

ProcessID.prototype._together_ = function() { return [this.process_id, this.node_id, this.global_id]; };
ProcessID.prototype.toString = function() { return "<" + this._together_.join('.') + ">"; };

exports.ProcessID = ProcessID;
