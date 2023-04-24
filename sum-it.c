// Level 3 - Exs 12

#include <stdio.h>

// For some reasone "inline" gives an error "Undefined symbols" during
// compilation
double _sumIt(size_t n, double xs[n]) {
  double ret = 0;
  for (size_t i = 0; i < n; ++i)
    ret += xs[i];
  return ret;
}

#define sumIt(n, ...) _sumIt((n), (double[(n)]){ __VA_ARGS__ })

int main(void) {
  double x = sumIt(3, 1.1, 2.2, 3.3);
  printf("x = %g\n", x);
}
