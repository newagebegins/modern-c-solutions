// Challenge 13 (Generic derivative)

#include <stdlib.h>
#include <tgmath.h>
#include <stdio.h>

typedef double real_fn(double);
typedef double complex complex_fn(double complex);

static double const dx = 1E-10;

double derivative_real(real_fn F, double x) {
  return (F(x+dx) - F(x))/dx;
}

double complex derivative_complex(complex_fn F, double complex x) {
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

double complex newton_complex(complex_fn F, double complex x) {
  double complex fx = F(x);
  if (cabs(fx) < dx) {
    return x;
  }
  double complex x2 = x - fx/derivative_complex(F, x);
  return newton_complex(F, x2);
}

double f1(double x) {
  return x*x*x - 4.0*x*x + 1.0;
}

double complex f2(double complex x) {
  return x*x*x - 4.0*x*x + 25.0*x - 100.0;
}

int main(void) {
  printf("f1(x) = x^3 - 4*x^2 + 1\n");
  double zer1 = newton_real(f1, 0.5);
  double res1 = f1(zer1);
  printf("f1(%g) = %g\n", zer1, res1);
  printf("\n");
  printf("f2(x) = x^3 - 4*x^2 + 25x - 100\n");
  double complex zer2 = newton_complex(f2, -7.0*I);
  double complex res2 = f2(zer2);
  printf("f2(%g%+gi) = %g%+gi", creal(zer2), cimag(zer2), creal(res2), cimag(res2));
  return EXIT_SUCCESS;
}
