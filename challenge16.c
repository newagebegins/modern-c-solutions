#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>

enum { max_line = 100 };

int main(void) {
  char const* counted_words[] = {
    "hello",
    "foo",
    "computer",
  };
  size_t const num_counted_words = sizeof counted_words / sizeof counted_words[0];
  size_t counts[num_counted_words] = {0};

  char const* pattern = "[0-9]+";
  size_t re_count = 0;
  regex_t preg;
  int errcode = regcomp(&preg, pattern, REG_EXTENDED);
  if (errcode) {
    char errbuf[1000];
    regerror(errcode, &preg, errbuf, sizeof errbuf);
    printf("regcomp failed with error: %s", errbuf);
    return EXIT_FAILURE;
  }

  char const* to_replace = "vim";
  char const* replacement = "emacs";

  char line[max_line] = {0};
  char line_alnum[max_line] = {0};
  char line_replaced[max_line] = {0};
  char* words[max_line] = {0};
  size_t num_words = 0;

  while (fgets(line, max_line, stdin)) {
    // Copy line into line_alnum, but replace all non-alphanumeric
    // characters with zeros. Effectively we split one big string into
    // multiple zero-terminating words. Pointers to these words are
    // saved into words array.
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

    // Count words
    for (size_t i = 0; i < num_words; ++i) {
      for (size_t j = 0; j < num_counted_words; ++j) {
        if (!strcmp(words[i], counted_words[j])) {
          counts[j] += 1;
          break;
        }
      }
    }

    // Count all occurences of a regexp pattern
    size_t nmatch = 1;
    regmatch_t pmatch[nmatch];
    p = line;
    while (!regexec(&preg, p, nmatch, pmatch, 0)) {
      ++re_count;
      p += pmatch[0].rm_eo;
    }

    for (size_t i = 0; i < num_counted_words; ++i) {
      fprintf(stderr, "%s: %zu  ", counted_words[i], counts[i]);
    }
    fprintf(stderr, "%s: %zu", pattern, re_count);
    fprintf(stderr, "\n");
  }

  return EXIT_SUCCESS;
}
