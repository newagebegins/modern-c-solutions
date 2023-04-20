// Exs 47: Write a function fseekmax that uses intmax_t instead of
// long and achieves large seek values by combining calls to fseek.

#include <stdio.h>
#include <stdint.h>
#include <limits.h>

int fseekmax(FILE* stream, intmax_t offset, int whence) {
  if (!offset) {
    return fseek(stream, 0, whence);
  }
  if (offset > 0) {
    while (offset > 0) {
      long o = offset <= LONG_MAX ? offset : LONG_MAX;
      int res = fseek(stream, o, whence);
      if (res < 0) return res;
      whence = SEEK_CUR;
      offset -= LONG_MAX;
    }
    return 0;
  }
  while (offset < 0) {
    long o = offset >= LONG_MIN ? offset : LONG_MIN;
    int res = fseek(stream, o, whence);
    if (res < 0) return res;
    whence = SEEK_CUR;
    offset -= LONG_MIN;
  }
  return 0;
}
