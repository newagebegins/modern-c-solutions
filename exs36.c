#include <stdio.h>

int main(void) {
  printf("%g\n", 1.0E-13 + 1.0E-13);
  printf("%g\n", (1.0E-13 + (1.0E-13 + 1.0)) - 1.0);
}
