#include <stdlib.h>
#include <stdio.h>

void fib2rec(size_t n, size_t buf[2]) {
  if (n > 2) {
    size_t res = buf[0] + buf[1];
    buf[1] = buf[0];
    buf[0] = res;
    fib2rec(n-1, buf);
  }
}

size_t fib2(size_t n) {
  size_t res[2] = { 1, 1, };
  fib2rec(n, res);
  return res[0];
}

int main(int argc, char* argv[argc+1]) {
  if (argv[1]) {
    size_t n = atoi(argv[1]);
    size_t res = fib2(n);
    printf("fib(%zu) = %zu\n", n, res);
  }
  return EXIT_SUCCESS;
}
