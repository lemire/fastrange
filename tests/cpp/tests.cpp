#include "../../fastrange.h"

// this just checks that we can compile successfully

int main() {
  for(uint32_t x = 0; x < 100; ++x) fastrange32(x,5);
  for(uint64_t x = 0; x < 100; ++x) fastrange64(x,5);

}
