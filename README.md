# fastrange: A fast alternative to the modulo reduction

A common operation in software is to take a machine word
and map it to an integer value in a range [0,p) as fairly as possible.
That is, you want that if all values of the machine word are
equally likely then, as much as possible, all integer values in
[0,p) are (almost) equally likely. This is common in hashing and probabilistic
algorithms.

The standard approach to this problem is the modulo reduction (``x % p``).
Though a modulo reduction works fine and has several nice properties,
it can be slow even on recent processors because it relies on an
integer division.

Thankfully, there is a faster way: we can replace the modulo by a multiplication
followed by a shift.

Further reading : http://lemire.me/blog/2016/06/27/a-fast-alternative-to-the-modulo-reduction/


This library provides a single portable header file that you should
be able to just drop in your C/C++ projects. The API is simple:


```
#include "fastrange"

// given a value word, produces an integer in [0,p) without division
uint32_t fastrange32(uint32_t word, uint32_t p);
uint64_t fastrange64(uint64_t word, uint64_t p);
size_t fastrangesize(size_t word, size_t p);
int fastrangeint(int word, int p);
```

On most system, the header will define the ``FASTRANGE64`` macro to indicate
that 64-bit words are supported. 
