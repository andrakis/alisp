ALisp
=====

A Lisp implementation in C++ 14 (**alisp**) and Node.js (**alisp.js**).


What?
-----

A Lisp interpreter that focuses on speed and minimizing memory usage.

The C++ version is written in C++ 14 and implements the [copy-and-swap idiom](https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom) to avoid memory leaks and lifetime issues.

The Node.js version is written to be compatible with the C++ version, and code should run on either version without issue.


Why?
----

Scheme is all well and fun, but using a string for the main data type results in constant parsing/unparsing, and large memory usage for small types.
Additionally, since each Scheme cell (in our implementation) has all possible data types, each Cell requires a lot of setup/teardown and memory.

Our Lisp implementation specialises the data types, so each Cell takes only the memory needed for its type.


How (to build)?
---------------

* C++ version:

  * NetBeans 8+: This repository is also a NetBeans project. The project was created with NetBeans 8.2 and older versions of NetBeans may not work with it.

  * On Linux:

    * Standard compile: `make CONF=Release`

    * Cross Platform OpenRISC: `make CONF=Release CND_PLATFORM=or1000 CXX=or1k-linux-musl-g++`

    * Look for `alisp` in <code>dist/**configuration**/**platform**/</code>
    
      * Where **configuration** is one of `Debug` or `Release`
      
      * Where **platform** is the Makefile platform, either `Cygwin_x86_64-Windows` or `or1000`. This platform name was automatically generated by NetBeans IDE, and may not reflect your actual platform.

  * Visual Studio:

    * Open `alisp.vcxproj` and compile.

    * Look for `alisp.exe` in the `Debug` or `Release` directories.

* Node.js version:

  * In any console run: <code>node index *[options]*</code>

  * Try running the debug evaluator in itself:

    * <code>node index seval.lisp -s -t seval.lisp fac.lisp</code>

      * The `-s` switch silences the debugging output of one of the seval instances.

	  * The `-t` switch enables tail recursion
  
Pass `--help` on the command line for information on commandline options.

See the [lisp](https://github.com/andrakis/alisp/tree/master/lisp) directory for samples than can be run.
  
Useful Resources
----------------

The following resources were invaluable in overcoming the many obstacles C++ likes to put in your way:

* [The C++ slicing problem](https://stackoverflow.com/questions/45259360/can-we-outsmart-object-slicing) - avoiding object slicing using smart pointers

* [GCC not catching exceptions](https://stackoverflow.com/questions/2424836/exceptions-are-not-caught-in-gcc-program) - why your `catch` clauses aren't catching under gcc

History
-------

(Some versions not in separate commits)

* (Alisp.js 0.2.4 Platform0.4) - Added "next" feature (transition control to given function without creating new environment); added fastmacro feature (use an environment created at declaration not runtime); [seval.lisp](https://github.com/andrakis/alisp/blob/master/lisp/seval.lisp) updated with new features

* 0.68.9 - Platform 0.3 implemented for Node.js. Contains an evaluator written in alisp, and more sample code.

* 0.67 - Fixed macros not running, `set!` misbehaving (throwing exceptions), added a number of platform functions; added Node.js version of interpreter and platform.

* 0.66 - Add runtime bounds/arguments checking; StringCell has ListCell-like behaviours; memory leak fixed; added linenoise-ng; split into new repo

* 0.60 - Add timekeeping; debugging, parsing, and evaluation functions; load file specified on command line and execute

* 0.50 - Add standard library of functions; command line parsing; StringTypeCell attempts to parse into number values

* 0.40 - Added REPL, exception handling

* 0.32 - Working eval loop, parsing; G++ fixes; Makefile included

* 0.22 - G++ fixes. Now compiles under G++ using `std=c++14`. Valgrind reports no memory leaks.

* 0.2 - Working prototype. Cell type completely rewritten.

* 0.1 - Prototype
