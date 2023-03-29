#include <stdlib.h>
#include <math.h>
#include <stdio.h>

static double const dx = 1E-10;

int main(void) {
  double x, der, ref;
  x = 3.5;

  der = (sin(x+dx) - sin(x))/dx;
  ref = cos(x);
  printf("sin(x): %.10f\t%.10f\n", der, ref);

  der = (cos(x+dx) - cos(x))/dx;
  ref = -sin(x);
  printf("cos(x): %.10f\t%.10f\n", der, ref);

  der = (sqrt(x+dx) - sqrt(x))/dx;
  ref = 1/(2*sqrt(x));
  printf("sqrt(x): %.10f\t%.10f\n", der, ref);

  return EXIT_SUCCESS;
}
