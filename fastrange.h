/**
* (c) Daniel Lemire
* Apache License 2.0
*/
#ifndef INCLUDE_FASTRANGE_H
#define INCLUDE_FASTRANGE_H
#include <iso646.h> // mostly for Microsoft compilers
#include <stdint.h> // part of Visual Studio 2010 and better
#include <stddef.h> // for size_t in C
#include <limits.h> // for size_t in C

/**
* Given a value "word", produces an integer in [0,p) without division.
* The function is as fair as possible in the sense that if you iterate
* through all possible values of "word", then you will generate all
* possible outputs as uniformly as possible.
*/
static inline uint32_t fastrange32(uint32_t word, uint32_t p) {
	return (uint32_t)(((uint64_t)word * (uint64_t)p) >> 32);
}

#ifdef _MSC_VER
#include <intrin.h>// should be part of all recent Visual Studio
#pragma intrinsic(_umul128)
#endif


#define FASTRANGE64

#if defined(_MSC_VER) && defined(_WIN32)
#undef FASTRANGE64 // non-sensical on a strictly 32-bit platform
#endif

#ifdef FASTRANGE64

/**
* Given a value "word", produces an integer in [0,p) without division.
* The function is as fair as possible in the sense that if you iterate
* through all possible values of "word", then you will generate all
* possible outputs as uniformly as possible.
*/
static inline uint64_t fastrange64(uint64_t word, uint64_t p) {
#ifdef __SIZEOF_INT128__ // then we know we have a 128-bit int
	return (uint64_t)(((__uint128_t)word * (__uint128_t)p) >> 64);
#elif defined(_MSC_VER)
	// supported in Visual Studio 2005 and better
	uint64_t highProduct;
	_umul128(word, p, &highProduct); // ignore output
	return highProduct;
	unsigned __int64 _umul128(
		unsigned __int64 Multiplier,
		unsigned __int64 Multiplicand,
		unsigned __int64 *HighProduct
	);
#else
#pragma message "Your system is unrecognized, please report the issue."
#endif
}

#endif // FASTRANGE64

#ifndef UINT32_MAX
#define UINT32_MAX  (0xffffffff)
#endif

/**
* Given a value "word", produces an integer in [0,p) without division.
* The function is as fair as possible in the sense that if you iterate
* through all possible values of "word", then you will generate all
* possible outputs as uniformly as possible.
*/
static inline size_t fastrangesize(size_t word, size_t p) {
#if (SIZE_MAX == UINT32_MAX) || !defined(FASTRANGE64)
	return (size_t)fastrange32(word, p);
#else // assume 64-bit
	return (size_t)fastrange64(word, p);
#endif
}

/**
* Given a value "word", produces an integer in [0,p) without division.
* The function is as fair as possible in the sense that if you iterate
* through all possible values of "word", then you will generate all
* possible outputs as uniformly as possible.
*/
static inline int fastrangeint(int word, int p) {
#if (SIZE_MAX == UINT32_MAX) || !defined(FASTRANGE64)
	return (int)fastrange32(word, p);
#else // assume 64-bit
	return (int)fastrange64(word, p);
#endif
}

#endif
/**
* (c) Daniel Lemire
* Apache License 2.0
*/
#ifndef INCLUDE_FASTRANGE_H
#define INCLUDE_FASTRANGE_H
#include <iso646.h> // mostly for Microsoft compilers
#include <stdint.h> // part of Visual Studio 2010 and better
#include <stddef.h> // for size_t in C
#include <limits.h> // for size_t in C

/**
* Given a value "word", produces an integer in [0,p) without division.
* The function is as fair as possible in the sense that if you iterate
* through all possible values of "word", then you will generate all
* possible outputs as uniformly as possible.
*/
static inline uint32_t fastrange32(uint32_t word, uint32_t p) {
	return (uint32_t)(((uint64_t)word * (uint64_t)p) >> 32);
}

/**
* Given a value "word", produces an integer in [0,p) without division.
* The function is as fair as possible in the sense that if you iterate
* through all possible values of "word", then you will generate all
* possible outputs as uniformly as possible.
*/
static inline uint64_t fastrange64(uint64_t word, uint64_t p) {
#ifdef __SIZEOF_INT128__ // then we know we have a 128-bit int
	return (uint64_t)(((__uint128_t)word * (__uint128_t)p) >> 64);
#elif defined(_MSC_VER)
	// supported in Visual Studio 2005 and better
	uint64_t highProduct;
	_umul128(word, p, &highProduct); // ignore output
	return highProduct;
	unsigned __int64 _umul128(
		unsigned __int64 Multiplier,
		unsigned __int64 Multiplicand,
		unsigned __int64 *HighProduct
	);
#else
#pragma message "Your system is unrecognized, please report the issue."
#endif
}

#ifndef UINT32_MAX
#define UINT32_MAX  (0xffffffff)
#endif

/**
* Given a value "word", produces an integer in [0,p) without division.
* The function is as fair as possible in the sense that if you iterate
* through all possible values of "word", then you will generate all
* possible outputs as uniformly as possible.
*/
static inline size_t fastrangesize(size_t word, size_t p) {
#if SIZE_MAX == UINT32_MAX
	return (size_t)fastrange32(word, p);
#else // assume 64-bit
	return (size_t)fastrange64(word, p);
#endif
}

/**
* Given a value "word", produces an integer in [0,p) without division.
* The function is as fair as possible in the sense that if you iterate
* through all possible values of "word", then you will generate all
* possible outputs as uniformly as possible.
*/
static inline int fastrangeint(int word, int p) {
#if SIZE_MAX == UINT32_MAX
	return (int)fastrange32(word, p);
#else // assume 64-bit
	return (int)fastrange64(word, p);
#endif
}

#endif
