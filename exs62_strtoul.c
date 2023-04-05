#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

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
    return (isupper(a)) ? 10 + (a - 'A') : -1;
  }
}

unsigned long Strtoul_inner(char const s[static 1],
                            size_t i,
                            unsigned base) {
  unsigned long ret = 0;
  while (s[i]) {
    unsigned c = hexatridecimal(s[i]);
    if (c >= base) break;
    /* Maximal representable value for 64 bit is
       3w5e11264sgsf in base 36 */
    if (ULONG_MAX/base < ret) {
      ret = ULONG_MAX;
      errno = ERANGE;
      break;
    }
    ret *= base;
    ret += c;
    ++i;
  }
  return ret;
}

size_t find_prefix(char const s[static 1], size_t i, char const p[static 1]) {
  size_t j = 0;
  for (; s[i] && p[j] && s[i] == p[j]; ++i, ++j) {}
  return j;
}

unsigned long Strtoul(char const s[static 1], unsigned base) {
  if (base > 36u) {             /* Tests if base        */
    errno = EINVAL;             /* Extends the specification */
    return ULONG_MAX;
  }
  size_t i = strspn(s, " \f\n\r\t\v");  /* Skips spaces  */
  bool switchsign = false;              /* Looks for a sign */
  switch (s[i]) {
  case '-' : switchsign = true;
  case '+' : ++i;
  }
  if (!base || base == 16) {            /* Adjusts the base */
    size_t adj = find_prefix(s, i, "0x");
    if (!base) base = (unsigned[]){ 10, 8, 16, }[adj];
    i += adj;
  }
  /* Now, starts the real conversion */
  unsigned long ret = Strtoul_inner(s, i, base);
  return (switchsign) ? -ret : ret;
}

int main(void) {
  char* xs[] = {
    "-1234567890",
    "12345677890",
    "01234567",
    "0x35abcdef",
    "0x35ABCDEF",
  };
  for (size_t i = 0; i < sizeof xs / sizeof xs[0]; ++i) {
    printf("%s\n%lu\n%lu\n\n", xs[i], strtoul(xs[i], 0, 0), Strtoul(xs[i], 0));
  }
}
