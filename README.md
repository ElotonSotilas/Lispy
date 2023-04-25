# Lispy – C++ Lisp Interpreter

Lispy is a Lisp dialect interpreted in C++. Implemented with smart pointers.

Two methods are implemented: `print` and `+`. The language is not turing complete,
but it should suffice for now.

```lisp
(print (+ 1 2)) 
```

This will output 3.

Currently, the app only runs in the console (because of how I made my main function).
For the demo, I will provide an implementation using argv
to determine whether a file is used or the console.