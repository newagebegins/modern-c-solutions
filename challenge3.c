#include <stdlib.h>
#include <math.h>
#include <stdio.h>

// https://en.wikipedia.org/wiki/Gauss%E2%80%93Legendre_algorithm
int main(void) {
  double a = 1.0;
  double b = 1.0/sqrt(2.0);
  double t = 1.0/4.0;
  double p = 1.0;
  for (size_t i = 0; i < 5; ++i) {
    double pi = (a+b)*(a+b)/(4*t);
    printf("%zu: %.14f\n", i, pi);
    double aa = (a + b)/2.0;
    b = sqrt(a*b);
    t = t - (p*(a-aa)*(a-aa));
    p = 2*p;
    a = aa;
  }
  return EXIT_SUCCESS;
}
