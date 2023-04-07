#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "libs/stb_image_write.h"

#include <stdlib.h>
#include <tgmath.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct regionStats regionStats;
struct regionStats {
  size_t num_pixels;
  size_t sum_values;
};

size_t Find(size_t parent[], size_t i) {
  if (parent[i] == SIZE_MAX) {
    return i;
  }
  return Find(parent, parent[i]);
}

void FindReplace(size_t parent[], size_t i, size_t x) {
  if (i == x) {
    return;
  }
  if (parent[i] != SIZE_MAX) {
    FindReplace(parent, parent[i], x);
  }
  parent[i] = x;
}

size_t FindCompress(size_t parent[], size_t i) {
  size_t root = Find(parent, i);
  FindReplace(parent, i, root);
  return root;
}

void Union(size_t parent[], size_t i, size_t j) {
  size_t root = FindCompress(parent, i);
  FindReplace(parent, j, root);
}

int main(int argc, char* argv[argc+1]) {
  if (argc < 4) {
    printf("Usage: %s input-file output-file threshold\n", argv[0]);
    return EXIT_FAILURE;
  }

  int width, height, num_components;
  unsigned char *pixels = stbi_load(argv[1], &width, &height, &num_components, 1);

  if (!pixels) {
    printf("Failed to open %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  double threshold = strtod(argv[3], 0);
  int len = width * height;
  size_t tree[len];
  for (size_t i = 0; i < len; ++i) {
    tree[i] = SIZE_MAX;
  }
  regionStats stats[len];
  for (size_t i = 0; i < len; ++i) {
    stats[i].num_pixels = 1;
    stats[i].sum_values = pixels[i];
  }

  bool made_changes;
  do {
    made_changes = false;
    for (size_t i = 0; i < len; ++i) {
      size_t row = i / width;
      size_t col = i % width;

      if (col > 0) {
        size_t root1 = Find(tree, i);
        double avg_value1 = (double) stats[root1].sum_values / stats[root1].num_pixels;

        size_t left_neighbor = i - 1;
        size_t root2 = Find(tree, left_neighbor);
        double avg_value2 = (double) stats[root2].sum_values / stats[root2].num_pixels;

        if (root1 != root2 && fabs(avg_value1 - avg_value2) <= threshold) {
          made_changes = true;
          Union(tree, left_neighbor, i);
          stats[root2].num_pixels += stats[root1].num_pixels;
          stats[root2].sum_values += stats[root1].sum_values;
        }
      }

      if (row > 0) {
        size_t root1 = Find(tree, i);
        double avg_value1 = (double) stats[root1].sum_values / stats[root1].num_pixels;

        size_t top_neighbor = i - width;
        size_t root2 = Find(tree, top_neighbor);
        double avg_value2 = (double) stats[root2].sum_values / stats[root2].num_pixels;

        if (root1 != root2 && fabs(avg_value1 - avg_value2) <= threshold) {
          made_changes = true;
          Union(tree, top_neighbor, i);
          stats[root2].num_pixels += stats[root1].num_pixels;
          stats[root2].sum_values += stats[root1].sum_values;
        }
      }
    }
  } while (made_changes);

  for (size_t i = 0; i < len; ++i) {
    size_t root = Find(tree, i);
    unsigned char avg_value = stats[root].sum_values / stats[root].num_pixels;
    pixels[i] = avg_value;
  }

  int write_result = stbi_write_png(argv[2], width, height, 1, pixels, width);
  if (!write_result) {
    printf("Failed to write %s", argv[2]);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
