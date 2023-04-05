#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

double my_strtod(char str[]) {
  double res = 0.0;
  bool neg = str[0] == '-';
  size_t i = neg ? 1 : 0;
  for (; str[i] && str[i] != '.'; ++i) {
    size_t digit = str[i] - '0';
    res *= 10.0;
    res += digit;
  }
  if (str[i] == '.') {
    double frac = 0.0;
    for (i = strlen(str) - 1; str[i] != '.'; --i) {
      size_t digit = str[i] - '0';
      frac /= 10.0;
      frac += digit;
    }
    frac /= 10.0;
    res += frac;
  }
  if (neg) {
    res *= -1.0;
  }
  return res;
}

int main(void) {
  char* S[] = {
    "-123.45678905",
    "0.0025",
    "123",
  };

  for (size_t i = 0; i < (sizeof S / sizeof S[0]); ++i) {
    printf("%s\n%.12f\n%.12f\n\n", S[i], my_strtod(S[i]), strtod(S[i], 0));
  }

  return EXIT_SUCCESS;
}
