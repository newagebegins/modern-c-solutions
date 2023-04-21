// Challenge 17: Text Processor (with multibyte characters)

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

enum { max_text = 64 };
typedef struct blob blob;
struct blob {
  char text[max_text];
  blob* prev;
  blob* next;
};

enum { max_blobs = 64 };
blob blobs[max_blobs] = {0};

blob* get_blob() {
  for (size_t i = 0; i < max_blobs; ++i) {
    if (!blobs[i].text[0]) {
      return &blobs[i];
    }
  }
  printf("Out of blobs\n");
  exit(EXIT_FAILURE);
}

void free_blob(blob* b) {
  b->text[0] = 0;
  b->next = 0;
  b->prev = 0;
}

void split(blob *b, size_t i) {
  blob* b2 = get_blob();
  strcpy(b2->text, &b->text[i+1]);
  b->text[i+1] = 0;
  b2->prev = b;
  b2->next = b->next;
  b->next = b2;
}

void join(blob* b) {
  size_t len = strlen(b->text);
  assert(len + strlen(b->next->text) + 1 < max_text);
  strcpy(&b->text[len], b->next->text);
  blob* tmp = b->next;
  b->next = b->next->next;
  if (b->next) {
    b->next->prev = b;
  }
  free_blob(tmp);
}

// Forms one blob per line
void format(blob* b) {
  if (!b) {
    return;
  }
  char* n = strchr(b->text, '\n');
  if (n) {
    if (n[1]) {
      split(b, n - b->text);
    }
    format(b->next);
  } else {
    format(b->next);
    join(b);
  }
}

// Prints up to n characters
void print_n(blob* b, size_t n) {
  if (!b) {
    return;
  }
  size_t len = strlen(b->text);
  if (len <= n) {
    fputs(b->text, stdout);
    print_n(b->next, n-len);
  } else {
    char buf[max_text];
    assert(max_text > n);
    strncpy(buf, b->text, n);
    buf[n] = 0;
    fputs(buf, stdout);
  }
}

void print_all(blob* b) {
  if (!b) {
    return;
  }
  fputs(b->text, stdout);
  print_all(b->next);
}

int main(void) {
  blob* b1 = get_blob();
  strcpy(b1->text, "1st");

  blob* b2 = get_blob();
  strcpy(b2->text, " line\n2nd line");

  blob* b3 = get_blob();
  strcpy(b3->text, ", still 2nd line\n");

  blob* b4 = get_blob();
  strcpy(b4->text, "3rd\n4th line!!!!!!!\n5th ");

  blob* b5 = get_blob();
  strcpy(b5->text, "line");

  blob* b6 = get_blob();
  strcpy(b6->text, " still 5th\n");

  b1->next = b2;
  b2->prev = b1;

  b2->next = b3;
  b3->prev = b2;

  b3->next = b4;
  b4->prev = b3;

  b4->next = b5;
  b5->prev = b4;

  b5->next = b6;
  b6->prev = b5;

  printf("format test:\n");
  format(b1);
  blob* x = b1;
  printf("1: %s", x->text);
  x = x->next;
  printf("2: %s", x->text);
  x = x->next;
  printf("3: %s", x->text);
  x = x->next;
  printf("4: %s", x->text);
  x = x->next;
  printf("5: %s", x->text);
  assert(!x->next);

  printf("\nprint_n test:\n");
  print_n(b1, 13);

  printf("\n\nprint_all test:\n");
  print_all(b1);

  return EXIT_SUCCESS;
}
