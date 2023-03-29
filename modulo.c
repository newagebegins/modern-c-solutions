#include <stdio.h>

int main(void) {
  printf("3 hours after 10:00 is %d:00\n", (10+3)%24);
  printf("8 hours after 20:00 is %d:00\n", (20+8)%24);
  return 0;
}
