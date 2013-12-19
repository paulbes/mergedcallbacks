mergedcallbacks
===============

Class for merging callbacks with the same signature in C++ using boost::bind and boost::function.

Build: g++ -o main main.cpp MergedCallbacks.cpp
Run: ./main

Output from main:

f(), call counter: 0
A::f(int), call counter: 0
A::f(int), call counter: 1
B::g(int, int), call counter: 0
B::g(int, int), call counter: 1
B::g(int, int), call counter: 2
<executeCallbackWithPriority> merged the following invocations (with a priority of: NORMAL):
0xc4c5a0 => 1
0xc4c5c0 => 1
0xc4c600 => 1
0xc4c620 => 1
0xc4c640 => 1

