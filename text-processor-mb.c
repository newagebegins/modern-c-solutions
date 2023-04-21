// Challenge 17: Text Processor (with multibyte characters)

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <wchar.h>
#include <locale.h>

enum { max_text = 64 };
typedef struct blob blob;
struct blob {
  wchar_t text[max_text];
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
  wcscpy(b2->text, &b->text[i+1]);
  b->text[i+1] = 0;
  b2->prev = b;
  b2->next = b->next;
  b->next = b2;
}

void join(blob* b) {
  size_t len = wcslen(b->text);
  assert(len + wcslen(b->next->text) + 1 < max_text);
  wcscpy(&b->text[len], b->next->text);
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
  wchar_t* n = wcschr(b->text, '\n');
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
  size_t len = wcslen(b->text);
  if (len <= n) {
    fputws(b->text, stdout);
    print_n(b->next, n-len);
  } else {
    wchar_t buf[max_text];
    assert(max_text > n);
    wcsncpy(buf, b->text, n);
    buf[n] = 0;
    fputws(buf, stdout);
  }
}

void print_all(blob* b) {
  if (!b) {
    return;
  }
  fputws(b->text, stdout);
  print_all(b->next);
}

int main(void) {
  setlocale(LC_ALL, "");

  blob* b1 = get_blob();
  wcscpy(b1->text, L"1ая");

  blob* b2 = get_blob();
  wcscpy(b2->text, L" строка\n2ая строка");

  blob* b3 = get_blob();
  wcscpy(b3->text, L", всё ещё 2ая строка\n");

  blob* b4 = get_blob();
  wcscpy(b4->text, L"3я\n4я строка!!!!!!!\n5я ");

  blob* b5 = get_blob();
  wcscpy(b5->text, L"строка");

  blob* b6 = get_blob();
  wcscpy(b6->text, L" всё ещё 5я\n");

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
  wprintf(L"1: %ls", x->text);
  x = x->next;
  wprintf(L"2: %ls", x->text);
  x = x->next;
  wprintf(L"3: %ls", x->text);
  x = x->next;
  wprintf(L"4: %ls", x->text);
  x = x->next;
  wprintf(L"5: %ls", x->text);
  assert(!x->next);

  printf("\nprint_n test:\n");
  print_n(b1, 13);

  printf("\n\nprint_all test:\n");
  print_all(b1);

  return EXIT_SUCCESS;
}
