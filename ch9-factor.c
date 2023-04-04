#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

size_t get_smallest_prime_factor(size_t n) {
  assert(n > 1);
  if (n % 2 == 0) {
    return 2;
  }
  for (size_t i = 3; i*i <= n; i += 2) {
    if (n % i == 0) {
      return i;
    }
  }
  return n;
}

void factor(size_t N) {
  assert(N > 1);
  size_t prev_F = 0;
  printf("%zu:", N);
  while (N > 1) {
    size_t F = get_smallest_prime_factor(N);
    if (F == prev_F) {
      N /= F;
      continue;
    }
    printf(" %zu", F);
    N /= F;
    prev_F = F;
  }
  printf("\n");
}

int main(int argc, char* argv[argc+1]) {
  for (int i = 1; i < argc; ++i) {
    size_t N = atoi(argv[i]);
    factor(N);
  }
}
