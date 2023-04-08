// Exs 9, p.144
#include <stdlib.h>
#include <stdio.h>

void swap3(double* p0, double* p1, double* p2) {
  double tmp = *p0;
  *p0 = *p2;
  *p2 = *p1;
  *p1 = tmp;
}

int main(void) {
  double d0 = 0.0;
  double d1 = 1.1;
  double d2 = 2.2;
  printf("Before swap:\n");
  printf("d0 = %f, d1 = %f, d2 = %f\n", d0, d1, d2);
  swap3(&d0, &d1, &d2);
  printf("After swap:\n");
  printf("d0 = %f, d1 = %f, d2 = %f\n", d0, d1, d2);
  return EXIT_SUCCESS;
}
