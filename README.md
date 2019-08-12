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

On GCC: `g++ -std=c++14 *.cpp -O2`

On Visual Studio: Open `alisp.vcxproj` and compile.

History
-------

* 0.22 - G++ fixes. Now compiles under G++ using `std=c++14`. Valgrind reports no memory leaks.

* 0.2 - Working prototype. Cell type completely rewritten.

* 0.1 - Prototype
