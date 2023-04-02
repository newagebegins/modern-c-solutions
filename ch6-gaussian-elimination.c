#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

#define ROWS 3
#define COLS 4

bool close_to_zero(double x) {
  return fabs(x) < 0.00001;
}

void swap_rows(double m[ROWS][COLS], size_t r1, size_t r2) {
  for (size_t j = 0; j < COLS; ++j) {
    double tmp = m[r1][j];
    m[r1][j] = m[r2][j];
    m[r2][j] = tmp;
  }
}

size_t find_r_max(double m[ROWS][COLS], size_t r, size_t c) {
  size_t r_max = r;
  double max = m[r][c];
  for (size_t i = r+1; i < ROWS; ++i) {
    if (m[i][c] > max) {
      max = m[i][c];
      r_max = i;
    }
  }
  return r_max;
}

size_t find_pivot_column(double row[COLS]) {
  for (size_t j = 0; j < COLS; ++j) {
    if (!close_to_zero(row[j])) {
      return j;
    }
  }
  return SIZE_MAX;
}

void to_reduced_row_echelon(double m[ROWS][COLS]) {
  size_t r = 0;
  size_t c = 0;
  while (r < ROWS && c < COLS) {
    size_t r_max = find_r_max(m, r, c);
    if (close_to_zero(m[r_max][c])) {
      ++c;
    } else {
      if (r != r_max) {
        swap_rows(m, r, r_max);
      }
      for (size_t i = r+1; i < ROWS; ++i) {
        double f = -m[i][c]/m[r][c];
        m[i][c] = 0.0;
        for (size_t j = c+1; j < COLS; ++j) {
          m[i][j] += m[r][j]*f;
        }
      }
      ++r;
      ++c;
    }
  }
  for (int r = ROWS-1; r >= 0; --r) {
    size_t c = find_pivot_column(m[r]);
    if (c != SIZE_MAX) {
      double f = 1.0/m[r][c];
      m[r][c] = 1.0;
      for (size_t j = c+1; j < COLS; ++j) {
        m[r][j] *= f;
      }
      for (int i = r-1; i >= 0; --i) {
        double f = -m[i][c];
        m[i][c] = 0.0;
        for (size_t j = c+1; j < COLS; ++j) {
          m[i][j] += m[r][j]*f;
        }
      }
    }
  }
}

void printMatrix(double m[ROWS][COLS]) {
  for (size_t r = 0; r < ROWS; ++r) {
    for (size_t c = 0; c < COLS; ++c) {
      printf("%g ", m[r][c]);
    }
    printf("\n");
  }
}

int main(void) {
  double A[ROWS][COLS] = {
    {2,1,-1,8},
    {-3,-1,2,-11},
    {-2,1,2,-3},
  };
  printf("A:\n");
  printMatrix(A);
  to_reduced_row_echelon(A);
  printf("\nA reduced row echelon:\n");
  printMatrix(A);
  return EXIT_SUCCESS;
}
