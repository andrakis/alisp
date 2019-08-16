ALisp
=====

A Lisp implementation in C++ 14.


What?
-----

A Lisp interpreter that focuses on speed and minimizing memory usage.

It is written in C++ 14 and implements the [copy-and-swap idiom](https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom) to avoid memory leaks and lifetime issues.


Why?
----

Scheme is all well and fun, but using a string for the main data type results in constant parsing/unparsing, and large memory usage for small types.
Additionally, since each Scheme cell (in our implementation) has all possible data types, each Cell requires a lot of setup/teardown and memory.

Our Lisp implementation specialises the data types, so each Cell takes only the memory needed for its type.

How (to build)?
---------------

On Linux: `make CONF=Release`

On Linux - Cross Platform OpenRISC: `make CONF=Release CND_PLATFORM=or1000 CXX=or1k-linux-musl-g++`

On Visual Studio: Open `alisp.vcxproj` and compile.

Useful Resources
----------------

The following resources were invaluable in overcoming the many obstacles C++ likes to put in your way:

* [The C++ slicing problem](https://stackoverflow.com/questions/45259360/can-we-outsmart-object-slicing) - avoiding object slicing using smart pointers

* [GCC not catching exceptions](https://stackoverflow.com/questions/2424836/exceptions-are-not-caught-in-gcc-program) - why your `catch` clauses aren't catching under gcc

History
-------

* 0.40 - Added REPL, exception handling

* 0.32 - Working eval loop, parsing; G++ fixes; Makefile included

* 0.22 - G++ fixes. Now compiles under G++ using `std=c++14`. Valgrind reports no memory leaks.

* 0.2 - Working prototype. Cell type completely rewritten.

* 0.1 - Prototype
