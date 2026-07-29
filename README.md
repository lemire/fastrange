# fastrange: A fast alternative to the modulo reduction

A common operation in software is to take a machine word
and map it to an integer value in a range [0,p) as fairly as possible.
That is, you want that if all values of the machine word are
equally likely then, as much as possible, all integer values in
[0,p) are (almost) equally likely. This is common in hashing and probabilistic
algorithms.

In computing, the modulo operation finds the remainder after division of one number by another (called the modulus of the operation). Given two positive numbers, a and n, a modulo n (abbreviated as a mod n) is the remainder of the Euclidean division of a by n, where a is the dividend and n is the divisor.

The standard approach to this problem is the modulo reduction (``x % p``).
Though a modulo reduction works fine and has several nice properties,
it can be slow even on recent processors because it relies on an
integer division.

Thankfully, there is a faster way: we can replace the modulo by a multiplication
followed by a shift.

[It has accelerated some operations in Google's Tensorflow by 10% to 20%](https://github.com/tensorflow/tensorflow/commit/a47a300185026fe7829990def9113bf3a5109fed).

Further reading : http://lemire.me/blog/2016/06/27/a-fast-alternative-to-the-modulo-reduction/


See also:

* Daniel Lemire, [Fast Random Integer Generation in an Interval](https://arxiv.org/abs/1805.10941), ACM Transactions on Modeling and Computer Simulation, January 2019 Article No. 3 https://doi.org/10.1145/3230636


This library provides a single portable header file that you should
be able to just drop in your C/C++ projects. The API is simple:


```C
#include <fastrange.h>

// given a value word, produces an integer in [0,p) without division
uint32_t fastrange32(uint32_t word, uint32_t p);
uint64_t fastrange64(uint64_t word, uint64_t p);
size_t fastrangesize(size_t word, size_t p);
int fastrangeint(int word, int p);
```

All the functions are ``static inline``: there is no library to link
against, and including the header from several translation units is fine.

## Requirements

A C99 (or better) or C++ compiler with the standard ``<stdint.h>`` types.
The header is known to work with GCC, Clang, Visual Studio and the Intel
compiler, on 32-bit and 64-bit systems. On compilers offering a 128-bit
integer type (GCC, Clang) and on 64-bit Visual Studio, ``fastrange64`` is
division free; elsewhere it falls back to ``word % p``.

## Usage with CMake

fastrange ships a CMake package. However you obtain it, the target to link
against is always ``fastrange::fastrange``, which is header only and simply
adds the header to your include path:

```cmake
target_link_libraries(your_target PRIVATE fastrange::fastrange)
```

### With FetchContent (no installation required)

```cmake
include(FetchContent)
FetchContent_Declare(fastrange
  GIT_REPOSITORY https://github.com/lemire/fastrange.git
  GIT_TAG        master)
FetchContent_MakeAvailable(fastrange)

target_link_libraries(your_target PRIVATE fastrange::fastrange)
```

### As a subdirectory (git submodule, vendored copy)

```cmake
add_subdirectory(third_party/fastrange)
target_link_libraries(your_target PRIVATE fastrange::fastrange)
```

### With an installed copy

```sh
cmake -B build
cmake --build build
ctest --test-dir build          # optional
cmake --install build --prefix /usr/local
```

then, from your project:

```cmake
find_package(fastrange 1.0 REQUIRED)
target_link_libraries(your_target PRIVATE fastrange::fastrange)
```

If fastrange is installed outside the default search path, point CMake at it
with ``-DCMAKE_PREFIX_PATH=/path/to/prefix``.

The installation also provides a ``pkg-config`` file for non-CMake build
systems:

```sh
cc myprogram.c $(pkg-config --cflags fastrange)
```

### Build options

| Option | Default | Meaning |
| --- | --- | --- |
| ``FASTRANGE_BUILD_TESTS`` | ``ON`` when fastrange is the top-level project, ``OFF`` otherwise | Build the tests and register them with CTest |
| ``FASTRANGE_INSTALL`` | ``ON`` when fastrange is the top-level project, ``OFF`` otherwise | Generate the install and package-config rules |

Because fastrange has no compiled component, the project declares no
language: adding it to your build does not force CMake to look for a C or a
C++ compiler. A compiler is only needed when ``FASTRANGE_BUILD_TESTS`` is on.

## Pre-conditions

For this code to give the desired result, the provided words should span the whole range (e.g., all 32-bit integers). The C ``rand`` function does not meet this requirement. If you must use the ``rand``  function, wrap it like so:

```
uint32_t get32rand() {
    return (rand() ^ (rand() << 15) ^ (rand() << 30));
}

uint64_t get64rand() {
    return (((uint64_t)get32rand()) << 32) | get32rand();
}
```


However, the underlying idea is general: we only require that the word values span an interval of the form ``[0,1<<L)``. It suffices then to do `` ( x * range ) >> L `` to get a fair map from ``[0,1<<L)`` to ``[0,range)``. For example, if your word values span the interval [0,65536), then you could simply do ``( x * range ) >> 16``.


## Unbiased range functions

To generate an unbiased random number in a range, you can use an extension of this approach:

```C
uint32_t random_bounded(uint32_t range) {
  uint64_t random32bit =  random32(); //32-bit random number 
  multiresult = random32bit * range;
  leftover = (uint32_t) multiresult;
  if(leftover < range ) {
      threshold = -range % range ;
      while (leftover < threshold) {
            random32bit =  random32();
            multiresult = random32bit * range;
            leftover = (uint32_t) multiresult;
      }
   }
  return multiresult >> 32;
}
```

See http://lemire.me/blog/2016/06/30/fast-random-shuffling/
