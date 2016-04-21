LibCC
----------

LibCC is a very simple static library for building compilers and interpreters in the same vain as Jack Crenshaw's *Let's Build a Compiler* series, but aimed at a more modern C-style approach

Dependencies
------------

This project has next to no dependencies.

* C Compiler
* Make
* C Standard Library

Usage
------------

Most of the usage is covered `libcc.h`, but the basics are as follows:

* `look` is the lookahead character
* Use `libcc_init` to start processing a file
* `error`, `warning`, and `emit` all support `printf` variadic arguments
* `getname`, `getnumber`, and so on all read from the stream
* `peekname` can be used to take a look at the next identifier in the stream
* `match` and `expected` can be used to handle syntax requirements

Examples
--------

The best example I have is included with the library! Simply use `make run` and a simple C compiler will be built. Look at the source in the `test/` directory for more information.

Contributing
------------

Feel free to add issues for suggestions and needed improvements! This is a small and very young project, but any help is greatly appreciated. In addition, if you need help using it, don't be afraid to ask.
