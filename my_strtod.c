#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

size_t find_dot(char str[]) {
  size_t i = 0;
  for (; str[i]; ++i) {
    if (str[i] == '.') {
      break;
    }
  }
  return i;
}

double my_strtod(char str[]) {
  size_t d = find_dot(str);
  double res = 0.0;
  bool neg = str[0] == '-';
  for (size_t i = neg ? 1 : 0; str[i]; ++i) {
    if (str[i] == '.') {
      continue;
    }
    size_t digit = str[i] - '0';
    int p = (i<d) ? (d-1-i) : -(i-d);
    res += digit * pow(10.0, p);
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
