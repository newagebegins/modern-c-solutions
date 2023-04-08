#include <stdio.h>

int main(void) {
  double A[2] = { 0.0, 1.0, };
  double* p = &A[0];
  printf("element %g\n", *p); // Referencing object
  p += 3;                     // Invalid pointer addition
                              // Undefined behavior
  return 0;
}
