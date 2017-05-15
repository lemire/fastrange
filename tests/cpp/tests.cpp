#include <cassert>
#include <stdlib.h>
#include <set>
#include "../../fastrange.h"


/////////////////////
// rand returns a random integer between 0 and RAND_MAX
// where RAND_MAX is at least 32767
// Note: this means that (uint64_t)rand() << 32 | rand() is NOT a proper
// way to generate random 64-bit integers in general.
/////////////////////

static uint32_t get32rand() {
    return (((uint32_t)rand() << 0) & 0x0000FFFFul) |
           (((uint32_t)rand() << 16) & 0xFFFF0000ul);
}

static uint64_t get64rand() {
    return (((uint64_t)get32rand()) << 32) | get32rand();
}

void fill(int number) {
    std::set<int> set;
    while(set.size() < (size_t) number) {
        set.insert(fastrangesize(get64rand(), number));
    }
}

int main() {
  for(uint32_t x = 0; x < 1000000; ++x) assert(fastrange32(x,5)<5);
  for(uint64_t x = 0; x < 1000000; ++x) assert(fastrange64(x,5)<5);
  for(int x = 1; x < 1000; ++x) fill(x);
}
