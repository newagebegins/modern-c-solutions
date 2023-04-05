#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

enum { buf_max = 32, };

void output_stream(FILE* instream, char buffer[buf_max], bool with_line_numbers) {
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
}

int main(int argc, char* argv[argc+1]) {
  char buffer[buf_max] = { 0 };

  if (argc == 1) {
    output_stream(stdin, buffer, false);
    return EXIT_SUCCESS;
  }

  bool with_line_numbers = false;
  size_t files_args_start = 1;

  if (strcmp(argv[1], "-n") == 0) {
    with_line_numbers = true;
    files_args_start = 2;
  }

  if (!argv[files_args_start]) {
    output_stream(stdin, buffer, with_line_numbers);
    return EXIT_SUCCESS;
  }

  int ret = EXIT_FAILURE;
  for (int i = files_args_start; i < argc; ++i) { // Processes args
    FILE* instream = fopen(argv[i], "r");         // as filenames
    if (instream) {
      output_stream(instream, buffer, with_line_numbers);
      fclose(instream);
      ret = EXIT_SUCCESS;
    } else {
      /* Provides some error diagnostic. */
      fprintf(stderr, "Could not open %s: ", argv[i]);
      perror(0);
      errno = 0;                        // Resets the error code
    }
  }
  return ret;
}
