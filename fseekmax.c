// Exs 47: Write a function fseekmax that uses intmax_t instead of
// long and achieves large seek values by combining calls to fseek.

#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <inttypes.h>
#include <stdlib.h>
#include <sys/errno.h>

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

int main(int argc, char* argv[argc+1]) {
  if (argc < 3) {
    printf("Usage: %s path-to-file byte-offset\n", argv[0]);
    return EXIT_FAILURE;
  }
  FILE* stream = fopen(argv[1], "r");
  if (!stream) {
    perror(0);
    return EXIT_FAILURE;
  }
  intmax_t offset = strtoimax(argv[2], 0, 10);
  if (!offset && errno) {
    perror(0);
    return EXIT_FAILURE;
  }
  if (fseekmax(stream, offset, offset < 0 ? SEEK_END : SEEK_SET) < 0) {
    perror(0);
    return EXIT_FAILURE;
  }
  unsigned char bytes[10] = {0};
  if (!fread(&bytes, sizeof bytes[0], sizeof bytes / sizeof bytes[0], stream)) {
    if (feof(stream)) {
      printf("EOF\n");
      return EXIT_FAILURE;
    }
    if (ferror(stream)) {
      printf("Error reading file\n");
      return EXIT_FAILURE;
    }
    printf("Unknown error\n");
    return EXIT_FAILURE;
  }
  printf("Bytes read:\n");
  for (size_t i = 0; i < sizeof bytes / sizeof bytes[0]; ++i) {
    printf("%02hhx", bytes[i]);
  }
  printf("\n");
  return EXIT_SUCCESS;
}
