#include <stdlib.h>
#include <stdio.h>

void mul_mat_vec(double m[2][3], double v[3], double out[2]) {
  for (size_t r = 0; r < 2; ++r) {
    out[r] = 0.0;
    for (size_t c = 0; c < 3; ++c) {
      out[r] += m[r][c]*v[c];
    }
  }
}

int main(void) {
  double A[2][3] = {
    {1,-1,2},
    {0,-3,1},
  };
  double x[3] = {2,1,0};
  double res[2] = {0};
  mul_mat_vec(A, x, res);
  printf("[%g, %g]\n", res[0], res[1]);
  return EXIT_SUCCESS;
}
