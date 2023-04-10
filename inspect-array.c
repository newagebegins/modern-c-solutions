// Exs 24. Investigate array bytes.

#include <stdlib.h>
#include <stdio.h>

typedef union inspectArray inspectArray;
union inspectArray {
  unsigned arr[2];
  unsigned char bytes[sizeof(unsigned[2])];
};

int main(void) {
  inspectArray inspect = {
    .arr = { 0xAABBCCDD, 0x11223344 }
  };
  for (size_t i = 0; i < 2; ++i) {
    printf("arr[%zu] = 0x%x\n", i, inspect.arr[i]);
    printf("&arr[%zu] = %p\n", i, &inspect.arr[i]);
  }
  for (size_t i = 0; i < sizeof inspect.bytes; ++i) {
    printf("byte[%zu] = 0x%hhx\n", i, inspect.bytes[i]);
  }
  return EXIT_SUCCESS;
}
