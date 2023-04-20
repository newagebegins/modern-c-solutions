#include <stdio.h>

int main(void) {
  double a[3];
  if (scanf("%lg%*[ \t]%lg%*[ \t]%lg", &a[0], &a[1], &a[2]) < 3) {
    printf("not enough input values!\n");
  } else {
    for (size_t i = 0; i < 3; ++i) {
      printf("a[%zu] = %g\n", i, a[i]);
    }
  }
}
