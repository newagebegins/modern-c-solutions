// Challenge 13 (Generic derivative)

#include <stdlib.h>
#include <tgmath.h>
#include <stdio.h>

typedef double real_fn(double);

static double const dx = 1E-10;

double derivative_real(real_fn F, double x) {
  return (F(x+dx) - F(x))/dx;
}

double newton_real(real_fn F, double x) {
  double fx = F(x);
  if (fabs(fx) < dx) {
    return x;
  }
  double x2 = x - fx/derivative_real(F, x);
  return newton_real(F, x2);
}

double f1(double x) {
  return x*x*x - 4*x*x + 1;
}

int main(void) {
  printf("f1(x) = x^3 - 4*x^2 + 1\n");
  printf("f1(%g) = 0\n", newton_real(f1, 0.5));
  return EXIT_SUCCESS;
}
