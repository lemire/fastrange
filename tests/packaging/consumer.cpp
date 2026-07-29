#undef NDEBUG
#include <cassert>
#include <fastrange.h>

// Checks that the installed header is usable from C++ through the
// fastrange::fastrange CMake target.
//
// Only fastrange32 has a single well-defined result everywhere; fastrange64
// falls back to a modulo reduction when no 128-bit multiplication is
// available, so we merely check that it stays in range.

int main() {
  assert(fastrange32(0, 10) == 0);
  assert(fastrange32(0x80000000u, 10) == 5);
  assert(fastrange64(0, 10) == 0);
  assert(fastrange64(UINT64_C(0x8000000000000000), 10) < 10);
  assert(fastrangesize(0, 10) == 0);
  assert(fastrangesize((size_t)-1, 10) < 10);
  return 0;
}
