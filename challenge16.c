#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

enum { max_line = 100 };

int main(void) {
  char const* counted_words[] = {
    "hello",
    "foo",
    "computer",
  };
  size_t const num_counted_words = sizeof counted_words / sizeof counted_words[0];
  size_t counts[num_counted_words] = {0};

  char const* regexp = "[0-9]+";
  size_t regexp_count = 0;

  char const* to_replace = "vim";
  char const* replacement = "emacs";

  char line[max_line] = {0};
  char line_alnum[max_line] = {0};
  char line_replaced[max_line] = {0};
  char* words[max_line] = {0};
  size_t num_words = 0;

  while (fgets(line, max_line, stdin)) {
    char *p = line;
    char *q = line_alnum;
    bool was_alnum = false;
    num_words = 0;
    while (*p) {
      bool is_alnum = isalnum(*p);
      *q = is_alnum ? *p : 0;
      if (is_alnum && !was_alnum) {
        words[num_words] = q;
        ++num_words;
      }
      was_alnum = is_alnum;
      ++p;
      ++q;
    }
    *q = 0;

    for (size_t i = 0; i < num_words; ++i) {
      for (size_t j = 0; j < num_counted_words; ++j) {
        if (!strcmp(words[i], counted_words[j])) {
          counts[j] += 1;
          break;
        }
      }
    }

    for (size_t i = 0; i < num_counted_words; ++i) {
      fprintf(stderr, "%s: %zu  ", counted_words[i], counts[i]);
    }
    fprintf(stderr, "\n");
  }

  return EXIT_SUCCESS;
}
