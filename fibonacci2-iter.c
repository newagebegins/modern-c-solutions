#include <stdlib.h>
#include <stdio.h>

size_t fib2_iter(size_t n) {
  size_t buf[2] = { 1, 1, };
  for (; n > 2; --n) {
    size_t res = buf[0] + buf[1];
    buf[1] = buf[0];
    buf[0] = res;
  }
  return buf[0];
}

int main(int argc, char* argv[argc+1]) {
  if (argv[1]) {
    size_t n = atoi(argv[1]);
    size_t res = fib2_iter(n);
    printf("fib2_iter(%zu) = %zu\n", n, res);
  }
  return EXIT_SUCCESS;
}
