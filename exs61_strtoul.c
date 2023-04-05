#include <stdio.h>
#include <assert.h>

/* Supposes that lowercase characters are contiguous. */
static_assert('z'-'a' == 25,
              "alphabetic characters not contiguous");
#include <ctype.h>
/* Converts an alphanumeric digit to an unsigned */
/* '0' ... '9'  =>  0 ..  9u */
/* 'A' ... 'Z'  => 10 .. 35u */
/* 'a' ... 'z'  => 10 .. 35u */
/* Other values =>   Greater */
unsigned hexatridecimal(int a) {
  if (isdigit(a)) {
    /* This is guaranteed to work: decimal digits
       are consecutive, and isdigit is not
       locale dependent. */
    return a - '0';
  } else {
    /* Leaves a unchanged if it is not lowercase */
    a = toupper(a);
    /* Returns value >= 36 if not Latin uppercase */
    return 'A' <= a && a <= 'Z' ? 10 + (a - 'A') : -1;
  }
}

int main(void) {
  int xs[] = {'0', '7', '9', 'a', 'A', 'z', 'Z', 'f', 'F', '@'};
  for (size_t i = 0; i < sizeof xs / sizeof xs[0]; ++i) {
    printf("%c -> %u\n", xs[i], hexatridecimal(xs[i]));
  }
}
