#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

enum { buf_max = 32, };

int main(int argc, char* argv[argc+1]) {
  int ret = EXIT_FAILURE;
  char buffer[buf_max] = { 0 };
  bool with_line_numbers = true;
  if (argc < 2) {
      size_t line_number = 1;
      bool is_beginning_of_line = true;
      while (fgets(buffer, buf_max, stdin)) {
        if (with_line_numbers) {
          if (is_beginning_of_line) {
            fprintf(stdout, "%zu ", line_number);
            ++line_number;
          }
          is_beginning_of_line = buffer[strlen(buffer) - 1] == '\n';
        }
        fputs(buffer, stdout);
      }
      ret = EXIT_SUCCESS;
  } else {
    for (int i = 1; i < argc; ++i) {        // Processes args
      FILE* instream = fopen(argv[i], "r"); // as filenames
      if (instream) {
        size_t line_number = 1;
        bool is_beginning_of_line = true;
        while (fgets(buffer, buf_max, instream)) {
          if (with_line_numbers) {
            if (is_beginning_of_line) {
              fprintf(stdout, "%zu ", line_number);
              ++line_number;
            }
            is_beginning_of_line = buffer[strlen(buffer) - 1] == '\n';
          }
          fputs(buffer, stdout);
        }
        fclose(instream);
        ret = EXIT_SUCCESS;
      } else {
        /* Provides some error diagnostic. */
        fprintf(stderr, "Could not open %s: ", argv[i]);
        perror(0);
        errno = 0;                        // Resets the error code
      }
    }
  }
  return ret;
}
