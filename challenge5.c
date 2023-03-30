#include <stdlib.h>
#include <tgmath.h>
#include <stdio.h>

static double complex const dx = CMPLX(1E-10, 0);

double complex derivative(double complex F(double complex), double complex x) {
  return (F(x+dx) - F(x))/dx;
}

int main(void) {
  double complex x, der, ref;
  x = CMPLX(3.5, 8.11);

  der = derivative(csin, x);
  ref = ccos(x);
  printf("csin(x): %f%+fi\t%f%+fi\n", creal(der), cimag(der), creal(ref), cimag(ref));

  der = derivative(ccos, x);
  ref = -csin(x);
  printf("ccos(x): %f%+fi\t%f%+fi\n", creal(der), cimag(der), creal(ref), cimag(ref));

  der = derivative(csqrt, x);
  ref = 1.0/(2.0*csqrt(x));
  printf("csqrt(x): %f%+fi\t%f%+fi\n", creal(der), cimag(der), creal(ref), cimag(ref));

  return EXIT_SUCCESS;
}
