// Exs 23. Investigate bytes of a double* pointer.

#include <stdlib.h>
#include <stdio.h>

typedef union pointerInspect pointerInspect;
union pointerInspect {
  double* ptr;
  unsigned char bytes[sizeof(double*)];
};

int main(void) {
  double val = 123.4;
  pointerInspect inspect = { .ptr = &val };
  printf("ptr = %p\n", inspect.ptr);
  for (size_t i = 0; i < sizeof inspect.bytes; ++i) {
    printf("byte[%zu] = 0x%x\n", i, inspect.bytes[i]);
  }
  return EXIT_SUCCESS;
}
