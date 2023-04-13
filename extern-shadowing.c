// Listing 13.2. An example of shadowing with an *extern* variable
#include <stdio.h>

unsigned i = 1;

int main(void) {
  unsigned i = 2;       /* A new object */
  if (i) {
    extern unsigned i;  /* An existing object */
    printf("%u\n", i);
  } else {
    printf("%u\n", i);
  }
}
