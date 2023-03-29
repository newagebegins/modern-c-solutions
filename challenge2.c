#include <stdlib.h>
#include <math.h>
#include <stdio.h>

static double const dx = 1E-10;

double derivative(double F(double), double x) {
  return (F(x+dx) - F(x))/dx;
}

int main(void) {
  double x, der, ref;
  x = 3.5;

  der = derivative(sin, x);
  ref = cos(x);
  printf("sin(x): %.10f\t%.10f\n", der, ref);

  der = derivative(cos, x);
  ref = -sin(x);
  printf("cos(x): %.10f\t%.10f\n", der, ref);

  der = derivative(sqrt, x);
  ref = 1/(2*sqrt(x));
  printf("sqrt(x): %.10f\t%.10f\n", der, ref);

  return EXIT_SUCCESS;
}
