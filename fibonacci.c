#include <stdlib.h>
#include <stdio.h>

size_t fib(size_t n) {
  if (n < 3)
    return 1;
  else
    return fib(n-1) + fib(n-2);
}

int main(int argc, char* argv[argc+1]) {
  if (argv[1]) {
    size_t n = atoi(argv[1]);
    size_t res = fib(n);
    printf("fib(%zu) = %zu\n", n, res);
  }
  return EXIT_SUCCESS;
}
